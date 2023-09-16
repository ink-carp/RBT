// 私有类型，只能被有元类RBT访问
class Node{
    static const bool RED = false;
    static const bool BLACK = true;
    private:
    bool color;
    int data;
    Node * parent;
    Node * l_child;
    Node * r_child;
    Node(int d){
        data = d;
        color = Node::BLACK;
        parent = nullptr;
        l_child = nullptr;
        r_child = nullptr;
    }
    void set_color(bool c){color = c;}
    // 递归删除节点
    void set_parent(Node * p){
        parent = p;
    }
    void drop(){
        if(l_child){
            l_child->drop();
        }
        if(r_child){
            r_child->drop();
        }
        delete this;
    }
    friend class RBT;
};
class RBT
{
private:
    void rotate(bool direct);
    bool go_parent();
    bool go_left();
    bool go_right();
    void case0();
    void case1();
    void case2();
    void case3();
    void merge();
    void fix();
        // 自身就是叶节点时
    void delete_leaf(){
        if(current->parent->l_child && current == current->parent->l_child){
            go_parent();
            delete current->l_child;
            current->l_child=nullptr;
        }else{
            go_parent();
            delete current->r_child;
            current->r_child=nullptr;
        }
    }
    void delete_leaf(Node * leaf){
        if(leaf->parent->l_child && leaf == leaf->parent->l_child){
            leaf->parent->l_child = nullptr;
        }else{
            leaf->parent->r_child = nullptr;
        }
        delete leaf;
    }
    Node * root;
    Node * current;
public:
    static const bool LEFT = false;
    static const bool RIGHT = true;
    void insert(int);
    bool search(int);
    void remove(int);
    bool is_empty(){
        return root == nullptr;
    }
    RBT(){
        root = nullptr;
        current = nullptr;
    }
    ~RBT(){
        current = nullptr;
        delete root;
    }
};