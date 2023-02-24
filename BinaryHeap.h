#pragma once

#include <vector>


struct HeapElement
{
    int     uniqueId;
    double  weight;
};

class BinaryHeap
{
    private:
        std::vector<HeapElement> elements;
        int size;

        int contain(int uniqueId) const
        {
            int i;
            for(i=1 ; i <= size ; i++ ){
                if (elements[i].uniqueId == uniqueId) return i;
            }
            return -1;
        }
        
        void percUp(int i)
        {
            while ((i / 2) > 0){
                if (this->elements[i].weight < this->elements[i/2].weight){
                    struct HeapElement tmp = this->elements[i/2];
                    this->elements[i/2] = this->elements[i];
                    this->elements[i] = tmp;
                }
                i = i/2;
            }
    
        }
        void percDown(int i){
            
            while ((i*2) <= this->size){
                int mc;
                if(size == i*2) mc = i*2;
                else 
                {
                    mc = (elements[i*2].weight>elements[i*2+1].weight)? (i*2+1):(i*2);
                    
                }
                
                if (this->elements[i].weight > this->elements[mc].weight){
                    struct HeapElement tmp = this->elements[i];
                    this->elements[i] = this->elements[mc];
                    this->elements[mc] = tmp;
                }
                i = mc;
            }
        }

    protected:
    public:
        // Constructors & Destructor
                            BinaryHeap();
        //
        bool                Add(int uniqueId, double weight);
        bool                PopHeap(int& outUniqueId, double& outWeight);
        bool                ChangePriority(int uniqueId, double newWeight);
        int                 HeapSize() const;
};