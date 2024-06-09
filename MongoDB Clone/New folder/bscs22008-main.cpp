#include<iostream>
#include<fstream>
#include<string>
#include"MongoDB.h"
using namespace std;


int main()
{
    ios::ios_base::sync_with_stdio(0);
    cout.tie(NULL);
    cin.tie(NULL);

    MongoDB bookstore;
    bookstore.addCollection("books.bson");
    bookstore.addCollection("books2.bson");
    bookstore.viewCollection("books");

    return 0;
}
