#include <iostream>
#include<vector>
#include<map>
#include<math.h>
#include<memory>
using namespace std ;

class avl
{
    class node ;
public :
    typedef shared_ptr<node> node_t ;
private :
    class node
    {   
    public :
        int key ;
        string  value ;
        int height ;
    public :
        node(){}
        node(int key,string value)
        {
            this->key = key ;
            this->value = value ;
        }
        ~node(){} 
    public :
        node_t l ;
        node_t r ;
    } ;

private:
    node_t  root ;
    int size ;

public :
    avl()
    {   
        root = nullptr ;
        size =  0;
    }
    ~avl(){}
public :
    int  getsize(){return size ;}
    bool isEmpty(){return size == 0 ;}
    int getHeight(node_t node)
    {
        if(!node)
        {
            return 0 ; 
        }
        return node->height ;
    }
    
    void add(int key, string  value) 
    {
        root = add(root, key, value) ;
    }

    node_t add(node_t root, int key, string value)
    {
        if(!root)
        {
            size++ ;
            return make_shared<node>(key, value) ;
        }
        if(key < root->key)
        {
            root->l = add(root->l, key, value) ;
        }
        else if(key > root->key)
        {
            root->r = add(root->r, key, value) ;
        }

        else
        {
            root->value = value ;
        }
            
        //更新height
        root->height = 1+ max(getHeight(root->l), getHeight(root->r)) ;
        
        //计算平衡因子
        int banlanceFactor = getBanlenceFactor(root) ;
        if(abs(banlanceFactor) > 1)
        {
            cout << "unbanlance : "<< banlanceFactor << endl ;
        }

        //维护平衡性
        
        return root ;
    }

    int getBanlenceFactor(node_t node)
    {
        if(!node)
        {
            return 0 ;
        }
        return getHeight(node->l)-getHeight(node->r) ;
    }
    
    bool isBST()
    {
        vector<int>a ;
        isOrder(root, a) ;
        for(size_t i = 1; i< a.size();i++)
        {
            if(a[i-1] > a[i])
            {
                return false ;
            }
        }
        return true ;
    }
    
    bool isBanlance()
    {
        return isBanlance(root) ;
    }

    bool isBanlance(node_t root)
    {
        if(!root)
        {
            return true ;
        }
        int banlance = getBanlenceFactor(root) ;
        if(abs(banlance)>1)
        {
            return true ;
        }
        return isBanlance(root->l)&&isBanlance(root->r) ;
    }

    void isOrder(node_t root, vector<int>&a)
    {
        if(root == nullptr)  
        {
            return ;
        }
        isOrder(root->l, a) ;
        a.push_back(root->key) ;
        isOrder(root->r, a) ;
    }

    node_t getNode()
    {

    }
} ;

int main()
{
    return 0;
}

