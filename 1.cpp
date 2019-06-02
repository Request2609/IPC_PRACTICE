#include <iostream>
#include<map>
using namespace std;
int main()
{
    map<string, int> maps ;
    maps["1"] = 1 ;
    maps["2"] = 2 ;
    maps["3"] = 3 ;
    int ret = maps.count("5") ;
    printf("%d\n", ret) ;
    ret = maps.count("1") ;
    printf("%d\n", ret) ;
    return 0;

}

