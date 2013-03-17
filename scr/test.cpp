#include <iostream>
#include <fstream>
using namespace std;
int main(){
	int num=12;
	for(int i=3;i>=0;--i){
		cout<<num;
		num=num>>1;
	}
	cout<<endl;
	return 0;
}