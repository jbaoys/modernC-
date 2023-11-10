// THIS IS AUTO GENERATED CODE, PLEASE DO NOT EDIT!!
// GENERATED ON 2023/10/15, 20:24:44

#include "VMFUtility.hh"
#include <iostream>
using namespace std;

typedef intAttribute<int16_t, 10, 0, 1023> AUR_ID;

typedef intAttribute<int64_t, 42, 0, 28100000000000> TIME;

// Indicator with a single field - OFFSETPOSITION
typedef VMFOptionalAttribute<AUR_ID> AUR_ID_FPI;
typedef VMFOptionalAttribute<int> FPI;
typedef VMFOptionalGroup<AUR_ID> AUR_ID_GPI;

int main()
{
    //AUR_ID_GPI aa(123);
   AUR_ID_FPI a(123);
   a.encode(nullptr);
//FPI b(123);
   cout << "hello" <<  '\n';
   cout << std::is_base_of<VMFAttribute, int>::value << "\n";
   return 0;
}
