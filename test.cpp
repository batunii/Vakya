#include <iostream>
using namespace std;

class A2DD{
    private:
    int gx;
    int gy;
    
    public:
    A2DD(int x,int y){
        gx=x;
        gy=y;
    }
    void getSum(){
        cout<<gx+gy<<endl;
    }
};
int main(){
    A2DD obj(2,3);
    obj.getSum();
}