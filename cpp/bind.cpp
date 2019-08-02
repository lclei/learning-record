#include <iostream>
#include <string>
#include <functional>

using namespace std;
using namespace std::placeholders; // for _1, _2 ...

string f(string a1, string a2, string a3, string a4)
{
    return a1+a2+a3+a4;
}

int main(int argc, char *argv[])
{
    auto g  = bind(f,_1,"Two","Three","Four");
    cout << g("one");
    return 0;
}
