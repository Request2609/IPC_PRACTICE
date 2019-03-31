#include <iostream>
#include <iostream>
#include<map>
#include<memory>
using namespace std ;
//二叉搜索树的学习

class node
{
    
    public :
        shared_ptr<node>l ;
        shared_ptr<node>r ;
        int a ;
    public :
        node()
        {
            a = 0 ;
            l = NULL ;
            r = NULL ;
        }

        node(int e)
        {
            a = e ;
            l = NULL ;
            r = NULL ;
        }

        ~node()
        {
            l = NULL ;
            r = NULL ;
        }
};

class BST: public node 
{
private :
    shared_ptr<node>root ;
    int size ;
    int a ;
public : 
    BST()
    {
        node() ;
        root = NULL ;
        size = 0 ;
    }
    
    ~BST()
    {
    }

public :
    void build()
    {
        int e ;
        while(1)
        {
            cin >>e ;
            if(e != -1)
            {
                root = build(root, e) ;
            }
            else 
            {
                break ;
            }
        }
    }

    //添加新元素
    shared_ptr<node> build(shared_ptr<node>root, int e)
    {
        if(root == NULL)
        {
            size ++ ;

             shared_ptr<node>p = make_shared<node>(e) ;
            
             return p ;
        }

        else
        {
            if(e > root->a)
            {
                root->r = build(root->r, e) ;
            }

            else 
            {
                root->l = build(root->l, e) ;
            }
        }

        return root ;
    }       

    void print()
    {
        print(root) ;
        cout<<endl ;
    }

    void print(shared_ptr<node>root)
    {
           if(root)
           {        
                cout << root->a <<"   " ;
                print(root->l) ;
                print(root->r) ;
           }
    }
    
    //向树中插入元素
    void add(int e)
    {
        root = add(root, e) ;
        cout << "插入元素"<<e<<"后:" << endl ;
        print(root) ;
        cout <<endl ;
    }

    shared_ptr<node> add(shared_ptr<node>root, int e)
    {
        if(root == NULL)
        {
            return shared_ptr<node>(new node(e)) ;
        }
        else
        {
            if(e < root->a)
            {
                //父亲节点数据肯定小于e，而当前节点肯定大于e，将e和父亲节点互换
                if(root->l == NULL)
                {
                    root->l = add(root->l, e) ;
                }

                //要是e不小于当前节点的左孩子数据
                if(e > root->l->a)
                {
                    swap(root->a, e) ;
                    add(root->r , e) ;
                } 

                if(e <= root->l->a)
                {
                    add(root->l, e) ;
                }
            }

            else if(e > root->a)
            {   
                if(root->r == NULL)
                {
                    root->r = add(root->r, e) ;
                }

                if(root->r->a > e)
                {
                    swap(root->a, e) ;
                    add(root->l, e) ;
                }

                if(root->r->a <= e)
                {
                    add(root->r, e) ;
                }
            }

            else
            {
                add(root->r, e) ;
            }
        }

        return root ;
    }

    void swap(int& old_, int& new_)
    {
        int temp = old_ ;
        old_ = new_ ;
        new_ = temp ;
    }


    //删除元素，感觉不常用，所以偷懒不写了！！！
    //思路：
    //1.当有左右孩子时
    //将左子树的最大节点或者右子树的最小节点调到当前节点位置即可这里省略了....
    //2.当没有左右孩子时，将当前节点指向空
    //3.当仅有右孩子或者左孩子，将当前节点的父亲节点指向该节点的左子树或者右子树即可
};

int main()
{
    BST bst ;
    bst.build() ;
    bst.print() ;
    bst.add(40) ;
}

