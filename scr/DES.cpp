#include <iostream>
#include <vector>
#include <fstream>
#include "key.h"
//				class: DES
class DES{
private:
	char data[64];
	char rightbckup[32];
	vector<int> CreateIP();
	vector<int> CreateIIP();
	vector<int> CreateEP();
	vector<int> CreateSBox(int n);
	vector<int> CreateSPerTable();
public:
	DES(const char[]);
	DES(char[],int size);
	void IP();			// Intitial permutation
	void des_function(vector<char> key);     // f(right,key)
	void mixer();		// XORing the left and right part to get the modified left part
	void swap();		// for swapping
	void IIP();			// Final Permutation
	void print();
	void printData();
};
vector<int> DES::CreateIP(){
	//cout<<"\nEntered CreateIP";
	ifstream fin("../resource/ip.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
vector<int> DES::CreateIIP(){
	ifstream fin("../resource/iip.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
vector<int> DES::CreateEP(){
	ifstream fin("../resource/EP.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
DES::DES(char input[],int size){
	for(int i=0;i<size;++i){
		for(int j=0;j<8;++j){
			if(input[i]%2)
				data[(i*8)+j]='1';
			else
				data[(i*8)+j]='0';
			input[i]>>1;
		}
	}
	// Inserting padding chars
	if(size<8){
		std::cout<<"Need to put padding bits";
		for(int i=size;i<8;++i)
			for(int j=0;j<8;++j)
				data[(i*8)+j]='1';
	}
}
DES::DES(const char input[]){
	for(int i=0;i<64;++i)
		data[i]=input[i];
}
void DES::IP(){
	char newData[64];
	vector<int> IPTable=CreateIP();
	for(int i=0;i<64;++i)
		newData[i]=data[IPTable[i]-1];
	for(int i=0;i<64;++i)
		data[i]=newData[i];
}
void DES::IIP(){
	char newData[64];
	vector<int> IIPTable=CreateIIP();
	for(int i=0;i<64;++i)
		newData[i]=data[IIPTable[i]-1];
	for(int i=0;i<64;++i)
		data[i]=newData[i];
}
vector<int> DES::CreateSBox(int n){
	ifstream fin;
	std::vector<int> v;
	switch(n)
	{
		case 1:fin.open("../resource/sbox1.txt");
			break;
		case 2:fin.open("../resource/sbox2.txt");
			break;
		case 3:fin.open("../resource/sbox3.txt");
			break;
		case 4:fin.open("../resource/sbox4.txt");
			break;
		case 5:fin.open("../resource/sbox5.txt");
			break;
		case 6:fin.open("../resource/sbox6.txt");
			break;
		case 7:fin.open("../resource/sbox7.txt");
			break;
		case 8:fin.open("../resource/sbox8.txt");
			break;
		default:std::cout<<"Wrong Input";
	}
	int num;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return(v);
}
vector<int> DES::CreateSPerTable(){
	ifstream fin("../resource/SPer.txt");
	int num;
	vector<int> v;
	while(fin){
		fin>>num;
		v.push_back(num);
	}
	return v;
}
void DES::des_function(vector<char> key){
	char right[32];
	char newData[48];
	for(int i=0;i<32;++i)
		rightbckup[i]=data[32+i];
	// Getting the data from right part of the data to be encrypted
	for(int i=0;i<32;++i)
		right[i]=data[32+i];
	// Applying expansion P-Box on right 32 bits to 48 bits
	vector<int> EPBox=CreateEP();
	for(int i=0;i<48;++i){
		newData[i]=right[EPBox[i]-1];
	}
	// XORing of output from P-Box and the key
	for(int i=0;i<48;++i){
		if(key[i]==newData[i])
			newData[i]='0';
		else
			newData[i]='1';
	}
	// Applying S box on 48bits from the XORing. Each 6bits at a time
	for(int i=1;i<=8;++i){
		vector<int> SBox=CreateSBox(i);
		int row=0,col=0;
		if(newData[((i-1)*6)]=='0'){
			if(newData[((i-1)*6)+5]=='0')
				row=0;
			else
				row=1;
		}
		else
		{
			if(newData[((i-1)*6)+5]=='0')
				row=2;
			else
				row=3;
		}
		for(int j=1;j<=4;++j){
			if(newData[((i-1)*6)+j]=='1')
				col=col + (1<<(4-j));
		}
		int num=SBox[(row*16)+col];
		for(int j=3;j>=0;--j)
		{
			if(num%2==1)
			{
				right[((i-1)*4)+j]='1';
				num=num>>1;
			}
			else
			{
				right[((i-1)*4)+j]='0';
				num=num>>1;
			}
		}
	}
	// Applying Straight Permutation and copy back to original data
	vector<int> SPerTable=CreateSPerTable();
	for(int i=0;i<32;++i){
		data[32+i]=right[SPerTable[i]-1];
	}
}
void DES::mixer(){
	for(int i=0;i<32;++i){
		if(data[i]==data[32+i])
			data[i]='1';
		else
			data[i]='0';
	}
}
void DES::swap(){
	for(int i=0;i<32;++i){
		data[32+i]=data[i];
		data[i]=rightbckup[i];
	}
}
void DES::print(){
	std::cout<<endl;
	for(int i=0;i<32;++i){
		if(i%4==0)
			std::cout<<"  ";
		std::cout<<data[i];
	}
	for(int i=0;i<32;++i){
		if(i%4==0)
			std::cout<<"  ";
		std::cout<<rightbckup[i];
	}
	std::cout<<"\nRight part edited";
	for(int i=0;i<32;++i){
		if(i%4==0)
			std::cout<<"  ";
		std::cout<<data[32+i];
	}
}
void DES::printData(){
	std::cout<<endl;
	int num;
	for(int i=0;i<64;++i){
		if(i%4==0)
			num=0;
		if(data[i]=='1')
			num+=(1<<(3-(i%4)));
		if(i%4==3)
			std::cout<<hex<<num;
	}
}
int main(){
	DES obj("0000000100100011010001010110011110001001101010111100110111101111");
	keyGen kObj("0001001100110100010101110111100110011011101111001101111111110001");
	obj.IP();
	for(int i=1;i<17;++i){
		vector<char> key=kObj.GenerateKey(i);
		obj.des_function(key);
		obj.mixer();
		obj.swap();
	}	
	obj.IIP();
	obj.printData();
	std::cout<<"\n";
	return 0;
}