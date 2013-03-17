#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class keyGen
{
private:
	char key[56];
	vector<int> CreatePC1();
	int CreateShift(int);
	vector<int> CreatePC2();
public:
	keyGen(const char k[]);
	vector<char> GenerateKey(int n);
	void print();
};
keyGen::keyGen(const char k[]){
	vector<int> pc1=CreatePC1();
	// applying PC1 to get the 56bits from 64 bits
	for(int i=0;i<56;++i){
		key[i]=k[pc1[i]-1];
	}
	// cout<<"\nConstructor working fine";
}
vector<int> keyGen::CreatePC1(){
	ifstream fin("../resource/pc1.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
int keyGen::CreateShift(int n){
	ifstream fin("../resource/shift.txt");
	int sum=0,num;
	if(n<=16)
	for(int i=0;i<n;++i){
		fin>>num;
		sum+=num;
	}
	return sum;
}
vector<int> keyGen::CreatePC2(){
	ifstream fin("../resource/pc2.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
void keyGen::print(){
	for(int i=0;i<56;++i){
		if(i%7==0)
			cout<<"  ";
		cout<<key[i];
	}
}
vector<char> keyGen::GenerateKey(int n){
	char keyBack[56],temp;
	vector<char> retKey;
	for(int i=0;i<56;++i)
		keyBack[i]=key[i];
	int shift=CreateShift(n);
	// Doing left shift in the left part of the key shift times by doing 1 shift at a time.
	// If we want to do all the shift all at once then it would require more space and more
	// importantly variable amount of space that the programmer have to manage himself.
	for(int j=0;j<shift;++j){
		temp=keyBack[0];
		for(int i=0;i<27;++i){
			keyBack[i]=keyBack[i+1];
		}
		keyBack[27]=temp;
	}
	// Doing  left shift in the right part of the key shift time by doing 1 shift at a time.
	for(int j=0;j<shift;++j){
		temp=keyBack[28];
		for(int i=0;i<27;++i){
			keyBack[i+28]=keyBack[i+29];
		}
		keyBack[55]=temp;
	}
	// Applying pc2 to get the 48bits key from 56bits key
	vector<int> v=CreatePC2();
	for(int i=0;i<48;++i){
		retKey.push_back(keyBack[v[i]-1]);
	}
	return retKey;
}