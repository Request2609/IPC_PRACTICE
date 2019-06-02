#include <iostream>
#include<fstream>
using namespace std ;

int main()
{
    ofstream out("llo", ios::app) ;
    int i = 1 ;
    if(!out) {
       cout << "can't open the window file!" << endl ;
       exit(1) ;
    }
    char bb[100] ;

    out << "hhhhhhhhhhhh" << i; 
    
    out.close() ;
    int k ;
    char buf[100] ;
    ifstream in("llo", ios::in) ;
    if(!in) {
        cout << "不能打开文件！"<<endl ;
        exit(1) ;
    }
    else {
        in>>buf>>k ;
    }
    cout <<buf << k<<endl ;
    return 0;
}

