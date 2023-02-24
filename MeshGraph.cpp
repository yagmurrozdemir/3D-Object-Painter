#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    int i;
    int a, b;
    vertices.resize(vertexPositions.size());
    for(i = 0 ; i < vertexPositions.size() ; i++)
    {
        vertices[i].position3D = vertexPositions[i];
        vertices[i].id = i;
    }
    adjList.resize(vertexPositions.size());
    
    for(i = 0 ; i < edges.size() ; i++)
    {
        a = edges[i].vertexId0;
        b = edges[i].vertexId1;
        adjList[a].push_back(&vertices[b]);
        adjList[b].push_back(&vertices[a]);
    }
    
    
}

double MeshGraph::AverageDistanceBetweenVertices() const
{   
    double totalDistance = 0;
    int i;
    double avarageDistance;
    
    for(i = 0 ; i < adjList.size(); i++)
    {
    
        for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); ++it)
        {
                totalDistance += Double3::Distance(vertices[i].position3D, (*it)->position3D) ;
            
        }
    }
     
    avarageDistance = totalDistance / (2 * TotalEdgeCount());
    return  avarageDistance;
}

double MeshGraph::AverageEdgePerVertex() const
{
    double edgeCount = TotalEdgeCount();
    double vertexCount = vertices.size();
    double EdgePerVertex = edgeCount/vertexCount;
    return EdgePerVertex;
}

int MeshGraph::TotalVertexCount() const
{
    int vertexCount;
    vertexCount = vertices.size();
    return vertexCount;
}

int MeshGraph::TotalEdgeCount() const
{
    int edgeCount = 0;
    int i;
    for(i = 0 ; i < adjList.size(); i++)
    {
        edgeCount += adjList[i].size();
    }
    edgeCount = edgeCount/2;
    return edgeCount;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if((vertexId >= vertices.size()) || (vertexId < 0)) return -1;
    
    int edgeCount = adjList[vertexId].size();
    return edgeCount;
    
    
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,int vertexId) const
{
    outVertexIds.clear();
    if((vertexId >= vertices.size()) || (vertexId < 0)) return;
    
    for (std::list<Vertex*>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); ++it)
    {
        outVertexIds.push_back((*it)->id);
    }
    return;
    
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    outputColorAllVertex.clear();
    
    if((vertexIdFrom >= vertices.size()) || (vertexIdFrom < 0)) return;
    if((vertexIdTo >= vertices.size()) || (vertexIdTo < 0)) return; 
    
    outputColorAllVertex.resize(vertices.size());
    BinaryHeap heap;
    std::vector<double> distances;
    std::vector<int> pre;
    
    
    for (int i = 0 ; i < vertices.size() ; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].b = 0;
        outputColorAllVertex[i].g = 0;
        
        heap.Add(i, INFINITY);
        distances.push_back(INFINITY);
    }
    
    
    heap.ChangePriority(vertexIdFrom , 0);
    distances[vertexIdFrom] = 0;
    
    int outUniqueId;
    double outWeight;
    double distance;
    
    pre.resize(vertices.size(),-1);
    
    pre[vertexIdFrom] = -1;
    
    while(heap.HeapSize())
    {
        heap.PopHeap(outUniqueId,outWeight);
        
        for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); ++it)
        {
            distance = Double3::Distance(vertices[((*it)->id)].position3D, vertices[outUniqueId].position3D) + outWeight;
            
            if(distance < distances[((*it)->id)] )
            {
                distances[((*it)->id)] = distance;
                heap.ChangePriority(((*it)->id), distance);
                pre[((*it)->id)] = outUniqueId;
            }
        }
        
    }
    
   std::vector<int> path;
    int currentVertex = vertexIdTo;
    while (currentVertex != -1)
    {
        path.push_back(currentVertex);
        currentVertex = pre[currentVertex];
    }
    if(path.size() == 1)
        return;
    // for reversing path:
    std::vector<int> temp;
    int path_size = path.size();
    for(int y=0;y<path_size;y++){
        int var = path.back();
        temp.push_back(var);
        path.pop_back();
    }

    for (std::vector<int>::iterator it = temp.begin(); it != temp.end(); ++it)
    {
       
        outputColorAllVertex[*it].r = color.r;
        outputColorAllVertex[*it].g = color.g;
        outputColorAllVertex[*it].b = color.b;
   
    }
    
    
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    outputColorAllVertex.clear();
    
    if((vertexId>= vertices.size()) || (vertexId < 0)) return;
    
    outputColorAllVertex.resize(vertices.size());
    
    BinaryHeap heap;
    std::vector<int> visited(vertices.size(),0);
    std::vector<double> depth(vertices.size(),INFINITY);
    std::vector<double> distance(vertices.size(),INFINITY);
    
    int weight = 0;
    

    
    for (int i = 0 ; i < vertices.size() ; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].b = 0;
        outputColorAllVertex[i].g = 0;
    }
    
    depth[vertexId] = 0;
    distance[vertexId]=0;
    visited[vertexId] = 1;
    
    heap.Add(vertexId , weight++);
    
    int outUniqueId;
    double outWeight;
    
    
    
    while(heap.HeapSize())
    {
        heap.PopHeap(outUniqueId,outWeight);
        
        outputColorAllVertex[outUniqueId].r = color.r;
        outputColorAllVertex[outUniqueId].b = color.b;
        outputColorAllVertex[outUniqueId].g = color.g;
    
        
        
        if(depth[outUniqueId] < maxDepth)
        {
            for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); ++it)
            {
                
                if(!visited[((*it)->id)])
                {
                    distance[((*it)->id)] = Double3::Distance(vertices[((*it)->id)].position3D, vertices[outUniqueId].position3D) + distance[outUniqueId];
                    heap.Add(((*it)->id), weight++);
                    visited[((*it)->id)] = 1;
                    
                    depth[((*it)->id)] = depth[outUniqueId]+1;
                    
                    
                    outputColorAllVertex[((*it)->id)].r = color.r;
                    outputColorAllVertex[((*it)->id)].b = color.b;
                    outputColorAllVertex[((*it)->id)].g = color.g;
                }
                
            }
        }
        
    }
    
    if(type == FILTER_BOX)
    {
        for(int i = 0 ; i<outputColorAllVertex.size();i++)
        {
            if(depth[i] <= maxDepth)
            {
                if(distance[i] > alpha || distance[i] < (-alpha))
                {
                    outputColorAllVertex[i].r = 0;
                    outputColorAllVertex[i].b = 0;
                    outputColorAllVertex[i].g = 0;
                }
            }
        }
    }
    
    if( type == FILTER_GAUSSIAN)
    {
        for(int i = 0 ; i<outputColorAllVertex.size();i++)
        {
            if(depth[i] <= maxDepth)
            {
                double x = distance[i];
                double f = exp(-( pow ( x , 2 ) / pow ( alpha , 2)));
                outputColorAllVertex[i].r *= f;
                outputColorAllVertex[i].b *= f;
                outputColorAllVertex[i].g *= f;
                
            }
        }
    }
    return;
    
    
    
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    outputColorAllVertex.clear();
    
    if((vertexId>= vertices.size()) || (vertexId < 0)) return;
    
    outputColorAllVertex.resize(vertices.size());
    
    BinaryHeap heap;
    std::vector<int> visited(vertices.size(), 0);
    std::vector<double> depth(vertices.size(), INFINITY);
    std::vector<double> distance(vertices.size(), INFINITY);
    
    int weight = 0;
    
   
    
    for (int i = 0 ; i < vertices.size() ; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].b = 0;
        outputColorAllVertex[i].g = 0;
    }
    
    depth[vertexId] = 0;
    distance[vertexId]=0;
    visited[vertexId] = 1;
    
    heap.Add(vertexId , weight++);
    
    int outUniqueId;
    double outWeight;
    
    
    
    while(heap.HeapSize())
    {
        heap.PopHeap(outUniqueId,outWeight);
        
        outputColorAllVertex[outUniqueId].r = color.r;
        outputColorAllVertex[outUniqueId].b = color.b;
        outputColorAllVertex[outUniqueId].g = color.g;
    
        
        if(depth[outUniqueId] < maxDepth)
        {
            for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); ++it)
            {
                
                if(!visited[((*it)->id)])
                {
                    distance[((*it)->id)] = Double3::Distance(vertices[((*it)->id)].position3D, vertices[vertexId].position3D);
                    heap.Add(((*it)->id), weight++);
                    visited[((*it)->id)] = 1;
                    
                    depth[((*it)->id)] = depth[outUniqueId]+1;
                    
                    
                    outputColorAllVertex[((*it)->id)].r = color.r;
                    outputColorAllVertex[((*it)->id)].b = color.b;
                    outputColorAllVertex[((*it)->id)].g = color.g;
                }
                
            }
        }
        
    }
    
    if(type == FILTER_BOX)
    {
        for(int i = 0 ; i<outputColorAllVertex.size();i++)
        {
            if(depth[i] <= maxDepth)
            {
                if(distance[i] > alpha || distance[i] < (-alpha))
                {
                    outputColorAllVertex[i].r = 0;
                    outputColorAllVertex[i].b = 0;
                    outputColorAllVertex[i].g = 0;
                }
            }
        }
    }
    
    if( type == FILTER_GAUSSIAN)
    {
        for(int i = 0 ; i<outputColorAllVertex.size();i++)
        {
            if(depth[i] <= maxDepth)
            {
                double x = distance[i];
                double f = exp(-( pow ( x , 2 ) / pow ( alpha , 2)));
                outputColorAllVertex[i].r *= f;
                outputColorAllVertex[i].b *= f;
                outputColorAllVertex[i].g *= f;
                
            }
        }
    }
    return;
    
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}