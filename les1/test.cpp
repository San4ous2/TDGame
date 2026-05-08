#include <iostream>
#include <cstdint>

using namespace std;
 struct chel{
    int age;
    char sim;
    void old(){
        age++;
    };
};

 int summa(int a, int b){
    return a+b;
}


 void test (){

    for (int i = 1; i<10; i++){
        cout << "pepe";
    }
    int A [100];

    cout << sizeof(A)/sizeof(A[0]);
    chel c = {12};
    c.old();
    c.sim = 1488;
    cout << c.sim << endl;
    cout << c.age << endl;

    int a = 60;int b = 7;
    cout << summa(a,b) << endl;


 }



int conv(double temp,string wh){
    if(wh == "f"){
        return temp * 9 / 5+32;
    }
    if(wh == "k"){
        return temp + 459.67;
    }
}


int MIN(int a ,int b, int c){
    if(a<b && a<c ){
        return a;
    }else if(b<a && b<c){
            return b;
    }else{
        return c;
    }
}

int MAX(int a ,int b, int c){
    if(a>b && a>c ){
        return a;
    }else if(b>a && b>c){
            return b;
        }else{
        return c;
    }
}

int MEX (int a ,int b, int c){
    int mx = MAX(a,b,c);

    for (int i = 0; i<mx;i++){
        if((i<a && i!=b && i!= c) || (i<b && i!=a && i!= c) || (i<c && i!=a && i!=c)){
            return i;
        }
    }
}

struct klass{
    int Grade;
    int index;
};

struct Student{
    int AGE;
    klass Class;
};

int main(){
    /*double temp;
    string wh;
    string con;
    for (int i = 0; i == 0; i+=0){
        cin >> temp;
        cin >> wh;
        cout << conv(temp,wh)<< wh << endl;
        cout << "Do you want to continue?" << endl;
        cin >> con;
        if(con == "yes"){
            continue;
        }else{
            break;
        }
    }*/

    /*int a;
    int b;
    int c;
    cin>> a;
    cin >> b;
    cin >> c;
    cout << "Min is:" << MIN(a,b,c) << endl;
    cout << "Max is:" << MAX(a,b,c) << endl;
    cout << "Mex is:" << MEX(a,b,c) << endl;*/
Student st[1000];
    string con;
    int age;
    int grade;
    int ind;

    for (int i = 0; i < 1000; i+=1){
        cin >> age;
        st[i].AGE = age;
        cin >> grade;
        st[i].Class.Grade = grade;
        cin >> ind;
        st[i].Class.index = ind;
        cout << "Stop?" << endl;
        cin >> con;
        if(con == "yes"){
            break;
        }else{
            continue;
        }
    }
    for int (i = 0;i<sizeof(st)/sizeof(st[0]);i++){
        cout <<"Student"<< i << endl;
        cout << "Age"<< st[i].AGE << endl;
        cout << "Grade" << st[i].Class.Grade << endl;
        cout << "Index" << st[i].Class.index << endl;
        }
    }





