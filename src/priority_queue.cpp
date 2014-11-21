#include <Rcpp.h>
// [[Rcpp::plugins(cpp11)]]

#include <queue>
#include <vector>
#include <functional>
using namespace Rcpp;
using namespace std;
using std::priority_queue;
using std::vector;

class Node {
  public:
    int index;
    Node *left;
    Node *right;
    Node(int, Node*, Node*);
};

Node::Node(int i, Node *l, Node *r) {
  index = i;
  left = l;
  right = r;
}

void visitNodes(Node *node, string s, const std::function<void(Node *, string)> &f) {
  if (node->left != NULL) {
    visitNodes(node->left, s + "0", f);
  }
  if (node->right != NULL) {
    visitNodes(node->right, s + "1", f);
  }
  f(node, s);
}

// [[Rcpp::export]]
Environment huffman_encode(NumericVector probabilities) {
  typedef pair<double, Node*> Elt;
  priority_queue<Elt, vector<Elt>, greater<Elt> > pq;
  for (int i = 0; i != probabilities.size(); ++i) {
    pq.push(Elt(probabilities[i], new Node(i, NULL, NULL)));
  }
  
  while (pq.size() > 1) {
    Elt one = pq.top();
    pq.pop();
    
    Elt two = pq.top();
    pq.pop();
    
    double weight = one.first + two.first;
    Node* node = new Node(-1, one.second, two.second);
    pq.push(Elt(weight, node));
  }
  
  CharacterVector items = probabilities.names();
  Environment env = Environment::empty_env().new_child(true);
  
  visitNodes(pq.top().second, "", [&items, &env] (Node *n, string s) {
    if (n->index >= 0) {
      env.assign((const string) items[n->index], s);
    }
    delete n;
  });
  return env;
}
