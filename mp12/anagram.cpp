#include "anagram.h"
#include <iostream>
#include <fstream>

/***
An AnagramDict constructor. This Constructor takes in a filepath
and then creates a binary tree out of all the words inside the 
binary tree. 

Input: 
std::string filename - the path to the file

Output:
N/A
***/
AnagramDict::AnagramDict(std::string filename) {
    filename_ = filename;
    std::string path = "dictionaries/" + filename_;
    std::ifstream file;
    file.open(path.c_str());
    /**
        Your code goes below here
    **/
    if (!file.is_open()) { // error 
        std::cerr << "Error opening file: " << path << std::endl;
    }
    std::string word;
    while (file >> word) {
        // std::cout << std::endl;
        // std::cout << word << std::endl;

        addWord(word);
    }
    file.close();

}

/***
An AnagramDict member function. This function sorts the given word in 
lexicographical order

Input: 
std::string word - the word that needs to be sorted

Output:
std::string - the sorted version of the input
***/
std::string AnagramDict::sortWord(std::string word) {
    /**
        Your code goes below here
    **/
    std::sort(word.begin(), word.end());
    return word;
}

/***
An AnagramDict member function. This function adds a word to the tree.
If the words sorted version already exists in the tree add it to the linked
list of that node. If the sorted word does not exist in the tree create a new
node and insert it into the tree
Duplicated words should not be added to the tree.
Input: 
std::string word - the word that needs to inserted

Output:
N/A
***/
void AnagramDict::addWord(std::string word) {
    /**
        Your code goes below here
    **/
    std::string sorted = sortWord(word);
    //std::cout << word + " (" + sorted + "):"<< std::endl;
    Node<std::string, std::list<std::string> > *exist = tree.find_node(sorted); // Search for the sortedword in the tree
    if (exist) {
        std::list<std::string> newlist = exist->getData();
        std::list<std::string>::iterator k = newlist.begin();
        for (;k!=newlist.end(); k++) {
            if (word == *k) break;
        }
        if(k == newlist.end()) newlist.push_back(word);
        exist->setData(newlist);
        std::list<std::string> temp = exist->getData();
        //printf("%d\n",(int)exist->getData().size());
        // for (std::list<std::string>::iterator k = temp.begin(); k!=temp.end(); k++)
        //     std::cout << *k << " ";
        // std::cout << std::endl;
    } 
    else { // DNE
        std::list<std::string> newlist;
        newlist.push_back(word);
        Node<std::string, std::list<std::string> > *newNode = new Node<std::string, std::list<std::string> >(sorted, newlist);
        tree.insert_node(tree.getRoot(), newNode);
        // std::cout << "Create newNode: "+ word << std::endl;
        // printf("%d\n",(int)newNode->getData().size());
    }
}

/***
An AnagramDict member function. Does a preorder, postorder, or inorder traversal
and then prints out all the anagrams and words.

The output file should be the traversal order of the tree, but only the data on each line. 
View on wiki for more information. 

Input: 
std::string order - The type of order you want to traverse. Can be "pre", "post", "in"

Output:
N/A
***/
void AnagramDict::saveAnagrams(std::string order) {
    std::string path = "output/" + order+"_"+ filename_;
    std::ofstream file;
    file.open(path.c_str());
    if(!file.is_open()) {
        //create new file
        file.open(path.c_str(),std::ios::out);
    }
    /**
        Your code goes below here
    **/
    std::list< Node<std::string,std::list<std::string> > > l;
    if (order == "pre")
        tree.pre_order(tree.getRoot(), l);
    else if (order == "in")
        tree.in_order(tree.getRoot(), l);
    else if (order == "post")
        tree.post_order(tree.getRoot(), l);
    std::list< Node<std::string,std::list<std::string> > >::iterator it;
    for (it = l.begin(); it!=l.end(); it++) {
        std::list<std::string> temp = it->getData();
        for (std::list<std::string>::iterator k = temp.begin(); k!=temp.end(); k++)
            file << *k << " ";
        file << std::endl;
    }


    file.close();
}