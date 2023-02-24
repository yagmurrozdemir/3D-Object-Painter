#include "BinaryHeap.h"
#include <iostream>
#include <vector>

BinaryHeap::BinaryHeap()
{
    std::vector<HeapElement> elements;
    this->elements = elements;
    
    struct HeapElement dummy= {0,0};
    
    this->elements.push_back(dummy);
    size=0;
    
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    struct HeapElement copy = {uniqueId, weight};

    
    if (this->contain(uniqueId)!=-1) return false;
    
    
    this->elements.push_back(copy);
    size +=1;
    
    if(size == 1)
    {
        return true;
    }
    
    this->percUp(size);
    
    return true;
    
    
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if (size==0) return false;
    
    
    outUniqueId = this->elements[1].uniqueId;
    
    outWeight = this->elements[1].weight;
    
    if (size==1)
    {
        this->elements.pop_back();
        size=0;
        return true;
        
    }
    
    this->elements[1] = this->elements[size];
    
    this->size--;
    this->elements.pop_back();
    this->percDown(1);
    return true;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    int index = this->contain(uniqueId);
    if (index==-1) return false;
    double oldWeight = elements[index].weight;
    elements[index].weight = newWeight;
    if(elements[index].weight > oldWeight)  this->percDown(index);
    else this->percUp(index);
    return true;
}

int BinaryHeap::HeapSize() const
{
    return size;
}

