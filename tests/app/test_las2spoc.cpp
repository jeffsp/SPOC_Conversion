#include "las2spoc.h"
#include "spoc/spoc.h"
#include "spoc/test_utils.h"
#include "spoc_conversion/spoc_conversion.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace spoc::las2spoc_app;
using namespace spoc_conversion;

void test_las2spoc ()
{
    las_reader l ("./test_data/lidar/juarez50.las");
}

int main (int argc, char **argv)
{
    try
    {
        test_las2spoc ();
        return 0;
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
