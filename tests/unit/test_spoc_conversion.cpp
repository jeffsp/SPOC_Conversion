#include "spoc_conversion/spoc_conversion.h"
#include "spoc/app_utils.h"
#include "spoc/test_utils.h"
#include <iostream>

using namespace std;
using namespace spoc::app_utils;
using namespace spoc_conversion;

void test_spoc_conversion ()
{
    const auto f1 = read_las_file ("./test_data/lidar/juarez50.las");
    const auto f2 = read_spoc_or_las_file ("./test_data/lidar/juarez50.las");
    const auto f3 = read_spoc_or_las_file ("./test_data/lidar/juarez50.spoc");
    const auto f4 = read_spoc_or_las_file ("./test_data/lidar/juarez50.zpoc");
    VERIFY (f1.is_valid ());
    VERIFY (f2.is_valid ());
    VERIFY (f3.is_valid ());
    VERIFY (f4.is_valid ());
}

int main (int argc, char **argv)
{
    try
    {
        test_spoc_conversion ();
        return 0;
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
