#ifndef RBT_H
#define RBT_H
#include "RBT.h"
#include<cstdlib>
bool RBT::go_parent(){
    if(is_empty())
    {
        exit(-1);
    }
    if(current!=root){
        current = current->parent;
        return true;
    }
    return false;
}
bool RBT::go_left(){
    if(is_empty())
    {
        exit(-1);
    }
    if(current->l_child){
        current = current->l_child;
        return true;
    }
    return false;
}
bool RBT::go_right(){
    if(is_empty())
    {
        exit(-1);
    }
    if(current->r_child){
        current = current->r_child;
        return true;
    }
    return false;
}
bool RBT::search(int target){
    if(is_empty())
    {
        exit(-1);
    }
    current = root;
    while(true){
        if(current->data == target){
            return true;
        }
        else if (target > current->data && !go_right()){
            return false;
        }
        else if(target < current->data && !go_left()){
            return false;
        }
    }
}
void RBT::insert(int data){
    if(is_empty()){
        root = new Node(data);
    }else if(!search(data)){
        current = root;
        while (true)
        {
            // 往左边插入或往右边插入
            if(data > current->data && !go_right()){
                Node * temp =new Node(data);
                temp->set_color(Node::RED);
                temp->set_parent(current);
                current->r_child = temp;
                if(current->color == Node::BLACK){
                    if(current->l_child && current->l_child->color==Node::RED){
                        case1();
                    }else{
                        go_right();
                        case0();
                    }
                }else{
                    case3();
                }
                break;
            }else if(data < current->data && !go_left()){
                Node temp = Node(data);
                temp.set_color(Node::RED);
                temp.parent = current;
                current->l_child = &temp;
                if(current->color == Node::RED){
                    case2();
                }
                break;
            }
        }
    }
}
void RBT::remove(int target){
    if(search(target)){
        current = root;
        while(current->data != target){
            if(target>current->data){
                if(current->color == Node::BLACK){
                    merge();
                }
                go_right();
            }else{
                if(current->color == Node::BLACK && current->l_child->color == Node::BLACK){
                    merge();
                }
                go_left();
            }
        }
        
        if(current->color == Node::BLACK){
            merge();
        }
        // 优先和左节点交换值(前驱)
        if(current->l_child){
            Node* temp = current->l_child;
            if(temp->r_child){
                do
                {
                    temp=temp->r_child;
                } while (temp->r_child);
                //找到待交换的节点
            }
            current->data = temp->data;
            // 删除待交换节点
            delete_leaf(temp);
        }else if(current->r_child){
            // 待删除节点没有左节点但有右节点
            Node* temp = current->r_child;
            if(temp->l_child){
                do
                {
                    temp=temp->l_child;
                } while (temp->l_child);
                //找到待交换的节点
            }
            current->data = temp->data;
            // 删除待交换节点
            delete_leaf(temp);
        }
        else{
            delete_leaf();
        }
        fix();
    }
}
void RBT::fix(){
    while (current!=root)
    {
        bool condition1 = current->r_child && current->r_child->color == Node::RED;
        bool condition2 = current->l_child && current->l_child->color == Node::RED && \
                    current->l_child->l_child && current->l_child->l_child->color == Node::RED;
        if(condition1){
            case0();
        }
        if(condition2){
            go_left();
            case2();
        }
        // go_parent是安全的，无论当前节点是否为root
        go_parent();
    }
    bool condition1 = current->r_child && current->r_child->color == Node::RED;
    bool condition2 = current->l_child && current->l_child->color == Node::RED && \
                current->l_child->l_child && current->l_child->l_child->color == Node::RED;
    if(condition1){
        case0();
    }
    if(condition2){
        go_left();
        case2();
    }
}
void RBT::merge(){
    if(current == root){
        if(current->l_child){
            current->l_child->color=Node::RED;
        }
        if(current->r_child){
            current->r_child->color=Node::RED;
        }
    }else if(current->parent->color == Node::RED){
        current->parent->color = Node::BLACK;
        if(current->parent->r_child){
            current->parent->r_child->color = Node::RED;
            current->color = Node::RED;
        }
    }
}
// rotate 函数保证，current在函数执行前后不变,即使current成为了root
void RBT::rotate(bool direct){
    if(direct == LEFT){
        // current 就是 root的右红儿子
        if(current->parent == root){
            root->r_child = current->l_child;
            current->l_child = root;
            root = current;
        }else{
            Node * pp = current->parent->parent;
            if(current->parent == pp->l_child){
                current->parent->parent->l_child = current;
            }else{
                current->parent->parent->r_child = current;
            }
            current->parent->r_child = current->l_child;
            current->l_child = current->parent;
            current->parent = pp;
        }
    }else{
        // current 就是 root的左儿子时，不会进行旋转
        if(current->parent->parent!=nullptr){
            Node * pp = current->parent->parent;
            if(current->parent == pp->l_child){
                current->parent->parent->l_child = current;
            }else{
                current->parent->parent->r_child = current;
            }
            current->parent->l_child = current->r_child;
            current->r_child = current->parent;
            current->parent = pp;
        }
    }
}
// 黑父有一个右红儿子的情况，需保证current此时为黑父
void RBT::case0(){
    rotate(RBT::LEFT);
    current->set_color(Node::BLACK);
    current->l_child->set_color(Node::RED);
}
// 黑父同时拥有左红儿子和右红儿子
// 使用时应保证current此时为黑父
void RBT::case1(){
    while(true){
        current->set_color(Node::RED);
        current->l_child->set_color(Node::BLACK);
        current->r_child->set_color(Node::BLACK);
        // 无法找到父亲，说明当前为root节点
        if(!go_parent()){
            current->set_color(Node::BLACK);
            break;
        }else if(current->l_child->color == Node::RED && current->r_child->color == Node::RED){
            // 导致父节点也出现的两红的情况
            continue;
        }else if(current->r_child->color == Node::RED){
            // 父节点出现了右侧红儿子
            case0();
            break;
        }
    }
}
// 连续出现两个左红儿子时,转化为case1问题
// 应保证使用时，current为中间的左孩子
void RBT::case2(){
    rotate(RBT::RIGHT);
    current->set_color(Node::BLACK);
    current->l_child->set_color(Node::RED);
    current->r_child->set_color(Node::RED);
    case1();
}
// 自身为黑父左孩子，且孩子为右红
void RBT::case3(){
    rotate(RBT::RIGHT);
    go_parent();
    case2();
}

#endif