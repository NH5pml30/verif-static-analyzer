#include <fstream>
#include <llvm/Support/raw_ostream.h>

#include "DefectStorage.h"

using namespace std;

DefectStorage &DefectStorage::Instance()
{
    static DefectStorage instance;
    return instance;
}
