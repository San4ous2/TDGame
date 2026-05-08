#include <iostream>
#include <typeinfo>
using namespace std;


class DynamicArray{
private:
    int len;
    int cap;
    int *data;
public:
    DynamicArray(int v1){
        len = v1;
        cap = len + 1;
        data = new int[len];
    }
    ~DynamicArray(){
        delete[] data;
    }
    Push_back(int v){
        len += 1;
        data = (int*) realloc(data, len * sizeof(int));
        data[len] = v;
    }
    int get(int idx){
        if(idx > len){
            return -1;
        }
        return data[idx];
    }
    int Set(int idx,int v){
        if(len == cap || idx > cap ){
            return -1;
        }else if(idx > len){
            Push_back(v);
            return 1;
        }else{
            data[idx] = v;
            return 1;
        }
    }
};




int main(){
DynamicArray lop(10);

lop.Set(10,6);
cout << lop.get(10) << endl;
lop.Push_back(8);
}
