#include <Rcpp.h>

class Node {
  public:
    int index;
    Node *left;
    Node *right;
    Node(int, Node*, Node*);
};
std::unordered_map<std::string, Node*> internal_huffman_map(Rcpp::NumericVector probabilities);