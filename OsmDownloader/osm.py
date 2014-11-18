import http.client
import xml.etree.ElementTree as etree
import turtle
import sys

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

    nodes, ways = srcFilter.match(srcMap)
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
    for wayIdx in self.ways:
      way = self.ways[wayIdx]
      for idx, nd in enumerate(way.nodes):
        ndIdx = self.nodes[nd].idx
        if ndIdx not in self.neighbors:
          self.neighbors[ndIdx] = []
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

  def draw(self, w, h):
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
      #if 'highway' not in tags:
      #  continue
      line = self.getPolyline(way)
      for (x, y) in line:
        cursor.pencolor('black')
        x, y = norm(x, y)
        cursor.setpos(x, y)
        #cursor.dot()
        if not cursor.isdown():
          cursor.down()
      cursor.up()

    turtle.update()

    window.exitonclick()

class Filter:
  def __init__(self, permitsNodes, permitsWays):
    self.permitsNodes = permitsNodes
    self.permitsWays = permitsWays

  def match(self, inMap):
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
      if k in way.tags and v is way.tags[k]:
        return True
      elif k in way.tags and v is '*':
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

fileName = ''
if len(sys.argv) > 1:
  fileName = sys.argv[1]
else:
  fileName = 'tewksbury.osm'
a = Map(fileName = fileName)

f1 = Filter(None, [('highway', '*')])
f2 = Filter([('attribution', '*')], None)
b = Map(source = (a, f1))
c = Map(source = (a, f2))

a.draw(750, 750)
b.draw(750, 750)
c.draw(750, 750)


# Tewksbury Center: 42.610556, -71.234722
# North Campus: 42.65, -71.3