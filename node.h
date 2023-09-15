using namespace std;

struct node{
double probability = 0;
char letter;
bool isLeaf = true;
node* right = nullptr;
node* left = nullptr;
};