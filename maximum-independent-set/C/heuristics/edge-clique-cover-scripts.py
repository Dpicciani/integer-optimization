
# coding: utf-8

# In[1]:

import random as rd
import numpy as np
import os
import sys
import matplotlib.pyplot as plt


# In[2]:

# read data file
# if data comes from Maximum Clique Problem, one has to work with the complement graph
# generate permutations for the vertice labels
# based on the new vertice labels, update the edges
# generate the original clique cover
# generate the test clique cover based on the permutated vertice labels
# turn back the test cliaue cover for the original vertice labels
# compare the test cliaue cover with the original cliaue cover


#  # **Kellerman’s heuristic for finding a Clique Cover**

# In[3]:

class Graph:
    
    def __init__(self, n):
        self.n = n
        self.adjs = []
        self.edges = set()
        for i in range(n):
            self.adjs.append(set())
        
    def add_edge(self, v, w):
        if v == w:
            raise Exception("Cannot create loop edge in node %s" % v)
        self.adjs[v].add(w)
        self.adjs[w].add(v)
        if not (w,v) in self.edges:
            self.edges.add((v,w))
    
    def adj(self, v, w):
        return v in self.adjs[w]
        
    def get_clique_cover(self):
        return Graph.CliqueCoverCalculator(self).calculate()
    
    # See http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=756229C2CFB6112593F593DF6D1013CF?doi=10.1.1.103.7486&rep=rep1&type=pdf
    class CliqueCoverCalculator:
        def __init__(self, g):
            self.g = g
            
        def calculate(self):
            g = self.g
            cover = []
            
            # Loop invariant: C1...Ck cover all edges incident to vertices v,w <= i
            for i in range(g.n):
            
                # W contains the nodes j before i for which edges i,j need to be covered
                w = set([j for j in range(i) if g.adj(i,j)])
                self.debug("\ni=%s; W=%s; cover=%s" % (i, w, cover))
                
                # If there are no nodes before i adjacent to it, create a new clique with i
                if len(w) == 0:
                    cover.append([i])
                    self.debug(" Creating new clique from %s" % [i])
                
                # Try to add i to each of the existing cliques
                else:
                    # Remove from W neighbors j of i where {i, j} is already covered by a previous clique
                    for clique in cover:
                        if self.can_include(i, clique):
                            clique.append(i)
                            for j in clique:
                                if j in w: w.remove(j)
                            self.debug(" Adding %s to clique %s, W is now %s" % (i, clique, w))
                            if len(w) == 0: break
                                
                    # For the remaining edges, try to cover as many as possible at a time
                    while len(w) > 0:
                        maximal_clique = self.find_maximal_clique(cover, w)
                        new_clique = [j for j in maximal_clique if j in w]
                        new_clique.append(i)
                        cover.append(new_clique)
                        for j in maximal_clique: 
                            if j in w: w.remove(j)
                        self.debug(" Generated new clique %s, W is now %s" % (new_clique, w))
            
            self.check_valid(cover)
            return cover
        
        def can_include(self, node, clique):
            return all([self.g.adj(node, k) for k in clique])

        def find_maximal_clique(self, cover, w):
            maximal = None
            value = 0
            for clique in cover:
                intersection = [j for j in clique if j in w]
                if value < len(intersection):
                    maximal, value = clique, len(intersection)
            return maximal
        
        def check_valid(self, cover):
            for v,w in self.g.edges:
                if not any([(v in clique and w in clique) for clique in cover]):
                    raise Exception("Edge %s,%s is not present in cover %s" % (v,w,cover))
                    
            for clique in cover:
                for v in clique:
                    if not all([self.g.adj(v,w) for w in clique if v != w]):
                        raise Exception("Node %s is not adjacent to all nodes in clique %s" % (v, clique))
        
            
        def debug(self, str):
#             print str
            return
            
# if __name__ == "__main__":
#     # number of vertices
#     g = Graph(4) 
    
#     # manual addition of edges
#     g.add_edge(0,1) 
#     g.add_edge(1,2)
#     g.add_edge(1,3)
#     g.add_edge(2,3)
#     g.add_edge(0,2)
    
#     # run kellerman's algorithm
#     cover = g.get_clique_cover()
#     print "\nCover is:"
#     for c in cover: print c
   


# # **Functions to generate different covers for the same graph G** 

# In[4]:

class rawGraph:
    def __init__(self, nV, nE):
        self.numberOfVertices = nV
        self.numberOfEdges = nE
        self.V = range(nV)
        self.invV = range(nV)
        self.E = []

def readDataFile(filepath):
    with open(filepath, 'r') as fileP:
        print('getting data...')
        for line in fileP:
            if line[0] == 'p':
                sLine = line.split(" ")
                G = rawGraph(int(sLine[2]),int(sLine[3]))
            elif line[0] == 'e':
                sLine = line.split(" ")
                G.E.append([int(sLine[2])-1, int(sLine[1])-1])
    fileP.close()
    
    print('data retrieved.')
    return G

def getTheComplementGraph(G):
    # The edges are stored in ascending order of vertices
    cG = rawGraph(G.numberOfVertices, 0)
    cG.V = G.V
    cG.invV = G.invV
    nV = G.numberOfVertices
    adjMatrix = np.zeros([nV, nV], dtype=int)
    for e in G.E:
        adjMatrix[e[0]][e[1]] = 1
        
    for i in range(nV):
        for j in range(i+1,nV):
            if adjMatrix[i][j] == 0:
                cG.E.append([i,j])
    
    cG.numberOfEdges = len(cG.E)
    
    print "complement Graph generated." 
    return cG

def generateIntegerRandomVectorAndItsInverse(n):
    F = rd.sample(range(n), n)
    invF = range(n);
    for i in range(n):
        invF[F[i]] = i
        
    return F, invF

def updateRawGraphWithNewLabels(rawG, newLabels, invNewLabels):
    permRawG = rawGraph(rawG.numberOfVertices, rawG.numberOfEdges)
    permRawG.V = newLabels
    permRawG.invV = invNewLabels
    
    for i in range(rawG.numberOfEdges):
        permRawG.E.append([permRawG.V[rawG.E[i][0]], permRawG.V[rawG.E[i][1]]])
    return permRawG

def rawGraphToGraph(rawG):
    G = Graph(rawG.numberOfVertices)
    for edge in rawG.E:
        G.add_edge(edge[0],edge[1])
    return G

def permCoverToCover(permCover, invV):
    coverG = []
    for c in permCover:
        invC = [invV[i] for i in c]
        invC.sort()
        coverG.append(invC)
    return coverG

def verifyEqualityOfSameSizeCliques(i,l,c1,c2):
    if i < l:
        if c1[i] != c2[i]:
            return False
        elif i != l-1:
            return verifyEqualityOfSameSizeCliques(i+1,l,c1,c2)
        else: return True
        
    else:
        return False

def intersectionBetweenCovers(cover1, cover2):
    intersection = []
    for c1 in cover1:
        for c2 in cover2:
            l = len(c1)
            if l == len(c2):
                if verifyEqualityOfSameSizeCliques(0,l,c1,c2):
                    intersection.append(c1)

    return intersection

def storeCliqueCoverIntoAFile(filepath, G, cover):
    with open(filepath, 'w') as fileP:
        fileP.write(str(G.numberOfVertices) + '\n')
        fileP.write(str(len(cover)) + '\n')
        lastCover = cover[-1]
        for c in cover:
            if c is lastCover:
                fileP.write(str(len(c)) + ' ' + ' '.join([str(x) for x in c]))
            else:            
                fileP.write(str(len(c)) + ' ' + ' '.join([str(x) for x in c]) + '\n')
        
        
    fileP.close()
    
def getRawGraphFromFile(filepath):
    originalG = readDataFile(filepath)
    return getTheComplementGraph(originalG)


# # **Functions to handle features from the results**

# In[5]:

def storeArrayOfFeatureIntoAFile(filepath, feature, nV):
    with open(filepath, 'w') as fileP:
        fileP.write(str(nV) + '\n')
        fileP.write(' '.join([str(x) for x in feature]))
        
    fileP.close()
    
def readFeaturesFile(filepath):
    with open(filepath, 'r') as fileP:
        lines = fileP.readlines()
        for i,line in enumerate(lines):
            if i == 1:
                feature = line
    fileP.close()
    
    return feature

def storeComparisonFile(filepath, minIndex, minRelax, minHist, maxIndex, maxRelax, maxHist):
    with open(filepath, 'w') as fileP:
        fileP.write(str(minIndex) + '\n')
        fileP.write(str(maxIndex) + '\n')
        fileP.write(minRelax + '\n')
        fileP.write(maxRelax + '\n')
        fileP.write(minHist + '\n')
        fileP.write(maxHist)
    fileP.close()


# # ***Functions to expand cliques in formulation***

# In[33]:

class clique:
    def __init__(self,length):
        self.len = length
        self.nodes = []
        self.sum = 0;
        
def getFullAdjacencyMatrix(G):
    
    adj = np.zeros([G.numberOfVertices, G.numberOfVertices], dtype='int')
    for e in G.E:
        adj[e[0]][e[1]] = 1
        adj[e[1]][e[0]] = 1
        
    #print('adjacency matrix generated.')
    return adj

def getCliques(filepath):
    C = []
    with open(filepath, 'r') as fileP:
        lines = fileP.readlines()
        for i,line in enumerate(lines):
            if i > 1:
                for j,k in enumerate(line.split()):
                    if j == 0:
                        currentClique = clique(int(k))
                    else:
                        currentClique.nodes.append(int(k))
                C.append(currentClique)
    fileP.close()
    
    return C

def storeExpandedCliqueCoverIntoAFile(filepath, G, cover):
    with open(filepath, 'w') as fileP:
        fileP.write(str(G.numberOfVertices) + '\n')
        fileP.write(str(len(cover)) + '\n')
        lastCover = cover[-1]
        for c in cover:
            if c is lastCover:
                fileP.write(str(c.len) + ' ' + ' '.join([str(x) for x in c.nodes]))
            else:            
                fileP.write(str(c.len) + ' ' + ' '.join([str(x) for x in c.nodes]) + '\n')
        
        
    fileP.close()  
    
def getCliqueCover(G):
    mainCover = []    
    # sys.stdout.write("generating the main cover...")
    coverG = G.get_clique_cover()
    # sys.stdout.write(" done.\n")
    
    for cliqueVertices in coverG:
        cl = clique(len(cliqueVertices))
        cl.nodes = cliqueVertices
        cl.sum = sum(cliqueVertices)
        mainCover.append(cl)
        
    return mainCover

def expandCliqueCover(adj, numberOfVertices, cover):
    for i,j in enumerate(cover):
        for elementNotInC in range(numberOfVertices):
            if elementNotInC not in cover[i].nodes:
                if not 0 in adj[cover[i].nodes,elementNotInC]:
                    cover[i].nodes.append(elementNotInC)
                    cover[i].len += 1
                    cover[i].sum += elementNotInC
    return cover

def getCoverWithOriginalLabels(permCover, invV):
    coverG = []
    for c in permCover:
        invC = [invV[i] for i in c.nodes]
        invC.sort()
        
        cl = clique(c.len)
        cl.nodes = invC
        cl.sum = sum(invC)
        
        coverG.append(cl)
        
    return coverG

def createDirectory(dirPath):
    try:  
        os.makedirs(directory)
    except OSError:  
        print ("Creation of the directory %s failed. Maybe it already exists." % directory)
    else:  
        print ("Successfully created the directory: .\\%s" % directory)
        
def tryToImproveFormulation(coverA, coverB):
    for cB in coverB:
        allowToGathercB = True
        for cA in coverA:
            if (cB.len == cA.len) and (cB.sum == cA.sum):
                if verifyEqualityOfSameSizeCliques(0,cB.len,cB.nodes,cA.nodes):
                    allowToGathercB = False
                    #print "cB is equal to cA"
        if allowToGathercB:
            coverA.append(cB)
            
    return coverA

def sortCliqueCover(mainCoverG):
    
    sortedCover = []
    listOfLength = []
    for c in mainCoverG:
        listOfLength.append(c.len)
        
    sortedIndices = np.argsort([-x for x in listOfLength])
    
    for i in sortedIndices:
        sortedCover.append(mainCoverG[i])
        
    return sortedCover    

def getFirstCumulativeCliqueCover(numberOfVertices, cover):
    
    cumulativeCover = []
    markers = np.zeros(numberOfVertices, dtype='int')
    for c in cover:
        if 0 in markers:
            cumulativeCover.append(c)
            for i in c.nodes:
                markers[i] = 1
    
    return cumulativeCover

def getCoverMarkers(numberOfVertices, cover):
    
    markers = np.zeros(numberOfVertices, dtype='int')
    for c in cover:
        for i in c.nodes:
            markers[i] = 1
            
    return markers

def getCumulativeCliqueCoverByAddingVerticeGreedyCliques(numberOfVertices, cover):
    cumulativeCover = []
    markers = np.zeros(numberOfVertices, dtype='int')
    for c in cover:
        if 0 in markers[c.nodes]:
            cumulativeCover.append(c)
            for i in c.nodes:
                markers[i] = 1
    
    return cumulativeCover

def isThatACliqueCover(adj, cover):
    
    for c in cover:
        subMatrix = adj[np.ix_(c.nodes, c.nodes)]
        for i in range(c.len):
            for j in range(c.len):
                if i!=j and subMatrix[i,j] == 0:
                    print "it is not a clique"
                    print subMatrix
                    
    print "end clique verification"


# # ***Robust formulation***

# In[16]:

# MaximumCliqueDataFilenames = ["C500.9"]
MaximumCliqueDataFilenames = ["C250.9"]

# MaximumCliqueDataFilenames = ["brock200_2"]
# MaximumCliqueDataFilenames.append("brock200_4")
# MaximumCliqueDataFilenames.append("brock400_2")
# MaximumCliqueDataFilenames.append("brock400_4")
# MaximumCliqueDataFilenames.append("brock800_2")
# MaximumCliqueDataFilenames.append("brock800_4")

bestTest = {
    "C250.9"     : "2",
    "C500.9"     : "1",
    "brock200_2" : "16",
    "brock200_4" : "3",
    "brock400_2" : "2",
    "brock400_4" : "20",
    "brock800_2" : "13",
    "brock800_4" : "6"
}

for MaximumCliqueDataFilename in MaximumCliqueDataFilenames:
    # generate paths
    filepath = ("D:\\local-database\\integer_problems\\clique-instances\\cliques\\" + MaximumCliqueDataFilename + ".clq")
    directory = MaximumCliqueDataFilename + "\\"
    
    # get different graph representations
    rawG = getRawGraphFromFile(filepath)
    G = rawGraphToGraph(rawG)
    adj = getFullAdjacencyMatrix(rawG)

    # generate the main cover
    mainCoverG = getCliqueCover(G)

    # expand the main clique cover
    mainCoverG = expandCliqueCover(adj, rawG.numberOfVertices, mainCoverG)

#     # get the main cover from file
#     expandedTestPath = directory + MaximumCliqueDataFilename + "_expanded_clique_cover_" + bestTest[MaximumCliqueDataFilename] + ".txt"
#     mainCoverG = getCliques(expandedTestPath)
    
    #     
    # Investigate new cliques
    #
    # - get a new cover
    # - expand it 
    # - try to add cliques to the main cover 
#
    mainCoverProgress = []
    mainCoverProgress.append(len(mainCoverG))
    numberOfPermutations = 1+1    
    for index in range(1,numberOfPermutations):
        # generate new vertice labels
        newLabels, invNewLabels = generateIntegerRandomVectorAndItsInverse(rawG.numberOfVertices)

        # update the graph representations with new labels
        permRawG = updateRawGraphWithNewLabels(rawG, newLabels, invNewLabels)
        permG = rawGraphToGraph(permRawG)
        permAdj = getFullAdjacencyMatrix(permRawG)

        # generate a clique cover
        coverPermG = getCliqueCover(permG)   

        # expand the new clique cover found
        coverPermG = expandCliqueCover(permAdj, rawG.numberOfVertices, coverPermG)
    
        # turn back to the original vertice label representation
        newCoverG = getCoverWithOriginalLabels(coverPermG, permRawG.invV)
    
        # try to improve the main clique cover
        mainCoverG = tryToImproveFormulation(mainCoverG, newCoverG)
    
        mainCoverProgress.append(len(mainCoverG))
    
        print [MaximumCliqueDataFilename, index, numberOfPermutations]

    # sort clique cover
    mainCoverG = sortCliqueCover(mainCoverG)
    print "main cover sorted."
    
    # get first cumulative clique cover
    cumulativeCover = getFirstCumulativeCliqueCover(rawG.numberOfVertices, mainCoverG)
    print "cumulative cover generated."
    
    # get cumulative clique cover by adding vertice greedy cliques
    cumulativeCoverWithGreedyCliques = getCumulativeCliqueCoverByAddingVerticeGreedyCliques(rawG.numberOfVertices, mainCoverG)
    print "cumulative greedy cover generated."
    
    strongerTestPath = directory + MaximumCliqueDataFilename + "_stronger_clique_cover_" + bestTest[MaximumCliqueDataFilename] + ".txt"
    storeExpandedCliqueCoverIntoAFile(strongerTestPath, rawG, mainCoverG)
    print "stored " + strongerTestPath
    
    cumulativeTestPath = directory + MaximumCliqueDataFilename + "_cumulative_clique_cover_" + bestTest[MaximumCliqueDataFilename] + ".txt"
    storeExpandedCliqueCoverIntoAFile(cumulativeTestPath, rawG, cumulativeCover)
    print "stored " + cumulativeTestPath
    
    cumulativeGreedyTestPath = directory + MaximumCliqueDataFilename + "_cumulative_greedy_clique_cover_" + bestTest[MaximumCliqueDataFilename] + ".txt"
    storeExpandedCliqueCoverIntoAFile(cumulativeGreedyTestPath, rawG, cumulativeCoverWithGreedyCliques)
    print "stored " + cumulativeGreedyTestPath
    
print "finished."


# In[ ]:

# sort clique cover
sortedCover = sortCliqueCover(mainCoverG)
print 'sorted.'

# get first cumulative clique cover
cumulativeCover = getFirstCumulativeCliqueCover(rawG.numberOfVertices, sortedCover)
print 'cumulative cover done.'

print getCoverMarkers(rawG.numberOfVertices, cumulativeCover)
print 'print markers done.'
print len(cumulativeCover)

# get cumulative clique cover by adding vertice greedy cliques
cumulativeCoverWithGreedyCliques = getCumulativeCliqueCoverByAddingVerticeGreedyCliques(rawG.numberOfVertices, sortedCover)
print 'cumulative clique cover by adding vertice greedy cliques done.'

print getCoverMarkers(rawG.numberOfVertices, cumulativeCoverWithGreedyCliques)
print 'print markers done.'
print len(cumulativeCoverWithGreedyCliques)

for c in cumulativeCoverWithGreedyCliques:
    print [c.len, c.nodes]

print rawG.numberOfVertices
print rawG.numberOfEdges
    
print 'finished.'


# # ***Debug***

# In[36]:

MaximumCliqueDataFilename = "C250.9"

bestTestDebug = {
    "C250.9"     : "1",
    "C500.9"     : "1",
    "brock200_2" : "16",
    "brock200_4" : "3",
    "brock400_2" : "2",
    "brock400_4" : "20",
    "brock800_2" : "13",
    "brock800_4" : "6"
}

directory = MaximumCliqueDataFilename + "\\"
expandedTestPath = directory + MaximumCliqueDataFilename + "_cumulative_greedy_clique_cover_" + bestTestDebug[MaximumCliqueDataFilename] + ".txt"
coverToBeAnalysed = getCliques(expandedTestPath)

isThatACliqueCover(adj, coverToBeAnalysed)


# In[44]:

plt.plot(mainCoverProgress)
plt.ylabel('Growth of the formulation')
plt.show()


# In[45]:

hist = np.zeros([np.round(rawG.numberOfVertices/2)], dtype='int')

for c in mainCoverG:
    hist[c.len] += 1
    
print hist

#  30: 209  4970 15689 12122  2892   226     4
# 100: 591 14810 46422 32735  6979   461     4
# 150: 7   138   574 44109  8629   532     4


# # **Main Script**

# In[ ]:

# MaximumCliqueDataFilename = "brock200_2"
# MaximumCliqueDataFilename = "brock200_4"
# MaximumCliqueDataFilename = "brock400_2"
# MaximumCliqueDataFilename = "brock400_4"
# MaximumCliqueDataFilename = "brock800_2"
MaximumCliqueDataFilename = "brock800_4"

filepath = ("D:\\local-database"
            "\\integer_problems"
            "\\clique-instances\\cliques"
            "\\" + MaximumCliqueDataFilename + ".clq")
originalG = readDataFile(filepath)

rawG = getTheComplementGraph(originalG)
G = rawGraphToGraph(rawG)

coverG = G.get_clique_cover()

directory = MaximumCliqueDataFilename + "\\"
try:  
    os.makedirs(directory)
except OSError:  
    print ("Creation of the directory %s failed. Maybe it already exists." % directory)
else:  
    print ("Successfully created the directory: .\\%s" % directory)

filepath = directory + MaximumCliqueDataFilename + "_clique_cover_0.txt"
storeCliqueCoverIntoAFile(filepath, rawG, coverG)
    
for index in range(1,21):
    newLabels, invNewLabels =     generateIntegerRandomVectorAndItsInverse(rawG.numberOfVertices)

    permRawG = updateRawGraphWithNewLabels(rawG, newLabels, invNewLabels)
    permG = rawGraphToGraph(permRawG)

    coverPermG = permG.get_clique_cover()    
    anotherCoverG = permCoverToCover(coverPermG, permRawG.invV)

    print("\n\ncover " + str(index))
    print("length: " + str(len(anotherCoverG)))
    intersection = intersectionBetweenCovers(coverG, anotherCoverG)
    print("Intersection between cover 0 and " + str(index) + " :")
    print(len(intersection))
    print(intersection)
    
#     filepath = directory + MaximumCliqueDataFilename + "_clique_cover_" + str(index) + ".txt"
#     storeCliqueCoverIntoAFile(filepath, permRawG, anotherCoverG)
    
    print("task finished.")


# # **Script to generate features from the results**

# In[9]:

_number_of_files = 1

MaximumCliqueDataFilenames = ["brock200_2"]
MaximumCliqueDataFilenames.append("brock200_4")
MaximumCliqueDataFilenames.append("brock400_2")
MaximumCliqueDataFilenames.append("brock400_4")
# MaximumCliqueDataFilenames.append("brock800_2")
# MaximumCliqueDataFilenames.append("brock800_4")

# MaximumCliqueDataFilenames = ["brock200_2"]
# MaximumCliqueDataFilenames = ["brock200_4"]
# MaximumCliqueDataFilenames = ["brock400_2"]
# MaximumCliqueDataFilenames = ["brock400_4"]
# MaximumCliqueDataFilenames = ["brock800_2"]
# MaximumCliqueDataFilenames = ["brock800_4"]

bestTest = {
    "brock200_2" : "16",
    "brock200_4" : "3",
    "brock400_2" : "2",
    "brock400_4" : "20",
    "brock800_2" : "13",
    "brock800_4" : "6"
}

for MaximumCliqueDataFilename in MaximumCliqueDataFilenames:

    directory = MaximumCliqueDataFilename + "\\"

    for index in range(_number_of_files):
#         resultspath = directory + MaximumCliqueDataFilename + "_clique_cover_" + str(index) + ".txt"
        
        resultspath = directory + MaximumCliqueDataFilename + "_stronger_clique_cover_" + bestTest[MaximumCliqueDataFilename] + ".txt"
    
        with open(resultspath, 'r') as fileP:
            for i, line in enumerate(fileP):
                if   i == 0:
                    nV = int(line)
                    hist = np.zeros(nV, dtype=int)
                    spect = np.zeros(nV, dtype=int)
                elif i == 1:
                    nC = int(line)
                else:
                    for j,k in enumerate(line.split()):
                        if j == 0:
                            hist[int(k)] += 1
                        else:
                            spect[int(k)] += 1

        fileP.close()
    
        histogramOfCliqueLengthPath = directory + MaximumCliqueDataFilename + "_stronger_clique_length_hist_" + bestTest[MaximumCliqueDataFilename] + ".txt"
#         frequencyOfVarInRestrictionspath = directory + MaximumCliqueDataFilename + "_var_res_freq_" + str(index) + ".txt"
    
        storeArrayOfFeatureIntoAFile(histogramOfCliqueLengthPath, hist, nV)
#         storeArrayOfFeatureIntoAFile(frequencyOfVarInRestrictionspath, spect, nV)

print 'all histograms and variables frequencies generated.'


# # ***Generate a comparison between the permutated formulation and the original one***

# In[85]:

MaximumCliqueDataFilenames = ["brock200_2"]
MaximumCliqueDataFilenames.append("brock200_4")
MaximumCliqueDataFilenames.append("brock400_2")
MaximumCliqueDataFilenames.append("brock400_4")
MaximumCliqueDataFilenames.append("brock800_2")
MaximumCliqueDataFilenames.append("brock800_4")

for filename in MaximumCliqueDataFilenames:
    relaxationFilePath = filename + "_clique_cover_result"
    directory = filename + "\\"
    
    # get relaxation data
    relaxation = []
    with open(relaxationFilePath, 'r') as fileP:
        for line in fileP:
            for i, k in enumerate(line.split()):
                if i == 2:
                    relaxation.append(k)
    fileP.close()                
    # get index of maximum and minimum relaxations                
    maxIndex = relaxation.index(max(relaxation))
    minIndex = relaxation.index(min(relaxation))
    
    # generate comparison file
    # relaxation 1
    # hist/var data
    # relaxation 2
    # hist/var data
    minRelaxHistPath = directory + filename + "_clique_length_hist_" + str(minIndex) + ".txt"
    maxRelaxHistPath = directory + filename + "_clique_length_hist_" + str(maxIndex) + ".txt"
    minfreqVarResPath = directory + filename + "_var_res_freq_" + str(minIndex) + ".txt"
    maxfreqVarResPath = directory + filename + "_var_res_freq_" + str(maxIndex) + ".txt"
    
    minHistFeat = readFeaturesFile(minRelaxHistPath)
    maxHistFeat = readFeaturesFile(maxRelaxHistPath)
    minVarFeat  = readFeaturesFile(minfreqVarResPath)
    maxVarFeat  = readFeaturesFile(maxfreqVarResPath)
    
    comparisonHistPath = directory + filename + "_comparison_min_max_hist.txt"
    comparisonVarPath = directory + filename + "_comparison_min_max_var.txt"
    
    print comparisonHistPath
    
    storeComparisonFile(comparisonHistPath, minIndex, relaxation[minIndex], minHistFeat, maxIndex, relaxation[maxIndex], maxHistFeat)
    storeComparisonFile(comparisonVarPath, minIndex, relaxation[minIndex], minVarFeat, maxIndex, relaxation[maxIndex], maxVarFeat)
    
print 'all comparison files generated.'


# # ***Expand clique formulation***

# In[46]:

# get full adjacency matrix of G
# get fomulation by cliques example
# expand all cliques in current formulation

# MaximumCliqueDataFilename = "brock200_2"
# MaximumCliqueDataFilename = "brock200_4"
# MaximumCliqueDataFilename = "brock400_2"
# MaximumCliqueDataFilename = "brock400_4"
# MaximumCliqueDataFilename = "brock800_2"
MaximumCliqueDataFilename = "brock800_4"

databaseFilepath = ("D:\\local-database"
                    "\\integer_problems"
                    "\\clique-instances\\cliques"
                    "\\" + MaximumCliqueDataFilename + ".clq")


minIndex = 6
cliqueCoverFilepath = (MaximumCliqueDataFilename + "\\" + MaximumCliqueDataFilename + "_clique_cover_" + str(minIndex) + ".txt")
expandedCliqueCoverFilepath = (MaximumCliqueDataFilename + "\\" + MaximumCliqueDataFilename + "_expanded_clique_cover_" + str(minIndex) + ".txt")

originalG = readDataFile(databaseFilepath)
rawG = getTheComplementGraph(originalG)

adj = getFullAdjacencyMatrix(rawG)

originalC = getCliques(cliqueCoverFilepath)
C = getCliques(cliqueCoverFilepath)

for i,j in enumerate(C):
    for elementNotInC in range(rawG.numberOfVertices):
        if elementNotInC not in C[i].nodes:
            if not 0 in adj[C[i].nodes,elementNotInC]:
                C[i].nodes.append(elementNotInC)
                C[i].len += 1
                
storeExpandedCliqueCoverIntoAFile(expandedCliqueCoverFilepath, rawG, C)

print 'expanded clique cover generated.'

