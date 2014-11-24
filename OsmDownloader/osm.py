import xml.etree.ElementTree as etree
import http.client
import math
import sys
import turtle

def getAngleRad(x, y):
  if x > 0 and y >= 0:
    return math.atan(y / x)
  elif x <= 0 and y > 0:
    return math.atan(x / y) + math.pi
  elif x < 0 and y <= 0:
    return math.atan(y / x) + math.pi
  elif x >= 0 and y < 0:
    return math.atan(x / y) + 2 * math.pi
  else:
    return 0



def getAngleDeg(x, y):
  return getAngleRad(x, y) * 180 / math.pi



class Map:
  def __init__(self, coords = None, fileName = None, source = None):
    self.n = 0
    self.s = 0
    self.e = 0
    self.w = 0

    self.nodes = {}
    self.ways = {}
    self.relations = {}
    self.neighbors = {}

    if coords != None:
      xml = self.__loadOnline(coords)      
      self.__parse(xml)
      self.__findNeighbors()
    elif fileName != None:
      xml = self.__loadLocal(fileName)      
      self.__parse(xml)
      self.__findNeighbors()
    elif source != None:
      srcMap, srcFilter = source
      self.__loadCopy(srcMap, srcFilter)
      self.__findNeighbors()

  def __loadOnline(self, coords):
    n, s, e, w = coords
    if n < s: n, s = s, n
    if e < w: e, w = w, e

    self.n = n
    self.s = s
    self.e = e
    self.w = w

    print('Loading api.openstreetmap.org/api/0.6/map?bbox={},{},{},{}'.format(w, s, e, n))
    conn = http.client.HTTPConnection('api.openstreetmap.org', 80)
    conn.request('GET', '/api/0.6/map?bbox={},{},{},{}'.format(w, s, e, n))
    response = conn.getresponse()

    if response.status != 200:
      print('{} {}'.format(response.status, response.reason))
      raise Exception('{} {}'.format(response.status, response.reason))

    xmlString = response.read().decode('utf-8')
    xmlTree = etree.fromstring(xmlString)
    return xmlTree

  def __loadLocal(self, fileName):
    xmlTree = etree.parse(fileName).getroot()
    bounds = xmlTree.findall('bounds')[0]
    self.n = float(bounds.attrib['maxlat'])
    self.s = float(bounds.attrib['minlat'])
    self.e = float(bounds.attrib['maxlon'])
    self.w = float(bounds.attrib['minlon'])
    return xmlTree

  def __loadCopy(self, srcMap, srcFilter):
    self.n = srcMap.n
    self.s = srcMap.s
    self.e = srcMap.e
    self.w = srcMap.w

    nodes, ways = srcFilter(srcMap)
    for node in nodes:
      copyNode = srcMap.nodes[node]
      self.nodes[node] = Node(copyNode.idx, copyNode.lat, copyNode.lon, copyNode.tags)
    for way in ways:
      copyWay = srcMap.ways[way]
      self.ways[way] = Way(copyWay.idx, copyWay.nodes, copyWay.tags)

  def __parse(self, xmlTree):
    for child in xmlTree:
      if child.tag == 'node':
        idx = int(child.attrib['id'])
        lat = child.attrib['lat']
        lon = child.attrib['lon']
        tags = {}
        for tag in child.findall('tag'):
          tags[tag.attrib['k']] = tag.attrib['v']
        self.nodes[idx] = Node(idx, lat, lon, tags)
      elif child.tag == 'way':
        idx = int(child.attrib['id'])
        nodes = []
        for node in child.findall('nd'):
          nodes.append(int(node.attrib['ref']))
        tags = {}
        for tag in child.findall('tag'):
          tags[tag.attrib['k']] = tag.attrib['v']
        self.ways[idx] = Way(idx, nodes, tags)
      elif child.tag == 'relation':
        1

  def __findNeighbors(self):
    for ndIdx in self.nodes:
      self.neighbors[ndIdx] = []
    for wayIdx in self.ways:
      way = self.ways[wayIdx]
      for idx, nd in enumerate(way.nodes):
        ndIdx = self.nodes[nd].idx
        if idx - 1 >= 0:
          self.neighbors[ndIdx].append(way.nodes[idx - 1])
        if idx + 1 < len(way.nodes):
          self.neighbors[ndIdx].append(way.nodes[idx + 1])

  def getNodes(self):
    return [i for i in self.nodes]

  def getWays(self):
    return [i for i in self.ways]

  def getPolyline(self, idx):
    verts = [(self.nodes[i].lon, self.nodes[i].lat) for i in self.ways[idx].nodes]
    return verts

  def draw(self, w, h, dot = False):
    norm = CoordinateNormalizer(self, w, h)
    window = turtle.Screen()
    cursor = turtle.Turtle()

    window.setup(w, h)
    window.setworldcoordinates(0, 0, w, h)
    window.delay(0)

    cursor.ht()

    turtle.tracer(0)

    cursor.up()
    for way in self.getWays():
      tags = self.ways[way].tags
      line = self.getPolyline(way)
      for (x, y) in line:
        cursor.pencolor('black')
        x, y = norm(x, y)
        cursor.setpos(x, y)
        if dot:
          cursor.dot()
        if not cursor.isdown():
          cursor.down()
      cursor.up()

    turtle.update()

    window.exitonclick()

  def resize(self, w, h):
    normalizer = CoordinateNormalizer(self, w, h)

    oldN, oldS, oldE, oldW = self.n, self.s, self.e, self.w
    self.n = h
    self.s = 0
    self.e = w
    self.w = 0

    for node in self.nodes:
      lon, lat = normalizer(self.nodes[node].lon, self.nodes[node].lat)
      self.nodes[node].lon, self.nodes[node].lat = lon, lat



class Filter:
  def __init__(self, permitsNodes, permitsWays):
    self.permitsNodes = permitsNodes
    self.permitsWays = permitsWays

  def __call__(self, inMap):
    ways = []
    if self.permitsWays is not None:
      ways = self.__matchWays(inMap.getWays(), inMap)
    else:
      ways = inMap.getWays()

    nodes = []
    if self.permitsNodes is not None:
      nodes = self.__matchNodes(inMap.getNodes(), inMap)
    else:
      nodes = inMap.getNodes()

    ways = self.__crossMatch(nodes, ways, inMap)
    return nodes, ways

  def __matchWays(self, ways, inMap):
    return [way for way in ways if self.__matchIndividualWay(inMap.ways[way])]

  def __matchIndividualWay(self, way):
    for k, v in self.permitsWays:
      if k in way.tags and v == way.tags[k]:
        return True
      elif k in way.tags and v == '*':
        return True
    return False

  def __matchNodes(self, nodes, inMap):
    return [node for node in nodes if self.__matchIndividualNode(inMap.nodes[node])]

  def __matchIndividualNode(self, node):
    for k, v in self.permitsNodes:
      if k in node.tags and v is node.tags[k]:
        return True
      elif k in node.tags and v is '*':
        return True
    return False

  def __crossMatch(self, nodes, ways, inMap):
    cullWays = []
    for way in ways:
      remainingNodes = [node for node in inMap.ways[way].nodes if node in nodes]
      if len(inMap.ways[way].nodes) is not len(remainingNodes):
        cullWays.append(way)
    return [way for way in ways if way not in cullWays]



class Node:
  def __init__(self, idx, lat, lon, tags):
    self.idx = int(idx)
    self.lat = float(lat)
    self.lon = float(lon)
    self.tags = tags

  def __repr__(self):
    return '{}: ({}, {}), {}'.format(self.idx, self.lat, self.lon, self.tags)



class Way:
  def __init__(self, idx, nodes, tags):
    self.idx = idx
    self.nodes = nodes
    self.tags = tags

  def __repr__(self):
    return '{}: {}, {}'.format(self.idx, self.nodes, self.tags)



class CoordinateNormalizer:
  def __init__(self, osmMap, scaleW, scaleH):
    self.rangeX = osmMap.w
    self.rangeY = osmMap.s
    self.rangeW = osmMap.e - osmMap.w
    self.rangeH = osmMap.n - osmMap.s
    self.scaleW = scaleW
    self.scaleH = scaleH

  def __call__(self, x, y):
    newX = (x - self.rangeX) / self.rangeW * self.scaleW
    newY = (y - self.rangeY) / self.rangeH * self.scaleH
    return (newX, newY)



class RoadBuilder:
  def __init__(self, width):
    self.width = width
    self.verts = []
    self.faces = []

  def __call__(self, srcMap):
    inCt = 0
    outCt = 0
    for node in srcMap.nodes:
      if node in srcMap.neighbors:
        inCt += 1
      else:
        outCt += 1
    self.__constructStraights(srcMap)
    self.__constructIntersections(srcMap)
    return (self.verts, self.faces)

  def __constructStraights(self, srcMap):
    1

  def __constructIntersections(self, srcMap):
    for node in srcMap.nodes:
      if len(srcMap.neighbors[node]) == 0:
        continue
      print(self.RoadIntersection(srcMap, node, self.width).getEnds())

  class RoadIntersection:
    def __init__(self, srcMap, ndIdx, width):
      self.ndIdx = ndIdx
      self.sortedNeighbors = srcMap.neighbors[ndIdx]
      self.lVerts = {}
      self.rVerts = {}

      c = (srcMap.nodes[ndIdx].lon, srcMap.nodes[ndIdx].lat)

      def getDeg(pt):
        x = srcMap.nodes[pt].lon - c[0]
        y = srcMap.nodes[pt].lat - c[1]
        return getAngleDeg(x, y)
      self.sortedNeighbors.sort(key = getDeg)

      offset = self.sortedNeighbors[1:]
      offset.append(self.sortedNeighbors[0])
      accum = 0
      for lIdx, rIdx in zip(self.sortedNeighbors, offset):
        lx = srcMap.nodes[lIdx].lon - c[0]
        ly = srcMap.nodes[lIdx].lat - c[1]
        rx = srcMap.nodes[rIdx].lon - c[0]
        ry = srcMap.nodes[rIdx].lat - c[1]
        angle = getAngleRad(rx, ry) - getAngleRad(lx, ly)
        if angle == 0:
          # guaranteed works
          la = getAngleRad(lx, ly) - math.pi / 2
          ra = getAngleRad(lx, ly) + math.pi / 2
          x1 = width * math.cos(la)
          y1 = width * math.sin(la)
          x2 = width * math.cos(ra)
          y2 = width * math.sin(ra)
          self.lVerts[lIdx] = (int(x1), int(y1))
          self.rVerts[lIdx] = (int(x2), int(y2))
        else:
          if angle < 0: angle += 2 * math.pi
          accum += angle
          magnitude = width / math.sin(angle / 2)
          x = magnitude * math.cos(getAngleRad(lx, ly) + angle)
          y = magnitude * math.sin(getAngleRad(lx, ly) + angle)
          self.lVerts[rIdx] = (int(x), int(y))
          self.rVerts[lIdx] = (int(x), int(y))

          if lIdx in self.lVerts and len(self.sortedNeighbors) == 2:
            x1, y1 = self.lVerts[rIdx]
            x2, y2 = self.lVerts[lIdx]

            #if x1 != -x2 or y1 != -y2:
            #  print('(rIdx ({}): (x: {}, y: {}, t: {}), lIdx ({}): (x: {}, y: {}, t: {})'.format(rIdx, x1, y1, angle, lIdx, x2, y2, 0))
            #  assert False
      if accum != math.pi * 2 and accum != 0:
        print(accum)
        assert False

    def getEndsByID(self, ndIdx):
      return (self.lVerts[ndIdx], self.rVerts[ndIdx])

    def getEnds(self):
      l = set([self.lVerts[ndIdx] for ndIdx in self.lVerts])
      r = set([self.rVerts[ndIdx] for ndIdx in self.rVerts])
      ret = list(l.union(r))

      def getDeg(pt):
        x = pt[0]
        y = pt[1]
        return getAngleDeg(x, y)
      ret.sort(key = getDeg)

      return ret

    def __repr__(self):
      outStr = ''
      for ndIdx in self.lVerts:
        outStr += '{} '.format(self.getEndsByID(ndIdx))
      return outStr

fileName = ''
if len(sys.argv) > 1:
  fileName = sys.argv[1]
else:
  fileName = 'tewksbury.osm'
a = Map(fileName = fileName)
a.resize(1000000, 1000000)

f1 = Filter(None, [('highway', '*')])
f2 = Filter([('attribution', '*')], None)
f3 = Filter(None, [('waterway', 'riverbank')])
b = Map(source = (a, f1))
c = Map(source = (a, f3))

#a.draw(750, 750)
#b.draw(750, 750)
#c.draw(750, 750, True)

r = RoadBuilder(20)
r(b)

# Tewksbury Center: 42.610556, -71.234722
# North Campus: 42.65, -71.3