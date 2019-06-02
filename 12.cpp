#include <iostream>
//运算符重载的时后要用到友元函数
//当类之间需要共享数据的时候
//许外面的类或函数去访问类的私有变量和保护变量，从而使两个类共享同一函数
//在实现类之间数据共享时，减少系统开销，提高效率
class X {
    friend void print() {
        X x ;
        x.test() ;
        std:: cout << "friend!"<< std::endl ;
    }

public :
    X(){}
    void test();
private :
    int y ;
} ;

//设置友元函数
void print() ;

void X::test() {
    std::cout << "hello firend!";
}

int main()
{
    print() ;   
    return 0;
}

