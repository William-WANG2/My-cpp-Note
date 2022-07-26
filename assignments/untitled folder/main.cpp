#include <iostream>
#include <string>
using namespace std;

class URL
{
private:
    string protocol;
    string resource;
public:
    URL(const string & protocol, const string & resource);
    ~URL();
    void print();
};

URL::URL(const string & protocol, const string & resource): protocol(protocol), resource(resource){};

URL::~URL()
{
}

void URL::print() {
    cout << this->protocol <<"://" << this->resource << endl;
}

int main() {
    URL url("http", "example.com/index.html");
	url.print();
}