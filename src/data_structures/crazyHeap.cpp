#include<vector>
#include<iostream>
#include<algorithm>
#include <tuple>
template<class tipo>
class crazyHeap{
	std::vector<tipo> heap;
	int getPai(int x){
		return (x-1)/2;
	}
	public:

		bool customComparator(const tipo& a,const tipo& b) {
		    return a.myUpperBound<b.myUpperBound;
		}

		
		bool empty(){
			return heap.empty();
		}
		tipo top(){
			return heap[0];
		}
		void heapify(int id){
			while(true){
				int left = 2*id+1;
				int right = 2*id+2;
				
				if(left>=heap.size())break;

				int teste;
				
				if(right>=heap.size()){
					teste = left;
				}
				else{
					if(customComparator(heap[left],heap[right])){
						teste = left;
					}
					else teste = right;
				}

				if(customComparator(heap[teste],heap[id])){
					std::swap(heap[teste],heap[id]);
					id = teste;
				}
				else break;

			}
		}
		void pop(){
			
			auto ultimo = heap.back();
			heap.pop_back();
			
			if(heap.empty())return;

			heap[0] = ultimo; //coloco em cima;

			int id = 0;
			
			heapify(0);
		}
		void push(tipo novo){
			heap.push_back(novo);
			int idAtual = heap.size()-1;
			while(idAtual!=0){
				if(customComparator(heap[idAtual],heap[getPai(idAtual)])){//se eu for menor que meu pai, swap
					std::swap(heap[idAtual],heap[getPai(idAtual)]);
					idAtual = getPai(idAtual);
				}
				else break;
			}
		}
		void relax(int x){
			//std::cout<<heap.size()<<std::endl;
			while(!heap.empty()){
				//std::cout<<heap.size()<<std::endl;
				if(heap.back().myLowerBound>=x)heap.pop_back();
				else break;
			}
		}
};


