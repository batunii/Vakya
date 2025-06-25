#include "A2DD.h"
using namespace std;

A2DD::A2DD(int x,int y){
    gx=x;
    gy=y;
}
void A2DD::getSum(){
        cout<<gx+gy;
}
int main(){
    A2DD obj(2,3);
    obj.getSum();
}