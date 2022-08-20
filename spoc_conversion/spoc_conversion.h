#pragma once

#include "spoc/spoc.h"
#include "lasreader.hpp"
#include <iostream>

namespace spoc_conversion
{

struct las_reader
{
    explicit las_reader (const std::string &fn)
        : lasreader (nullptr)
    {
        lasreadopener.set_file_name (fn.c_str ());
        lasreader = lasreadopener.open ();
        if (lasreader == nullptr)
            throw std::runtime_error ("Could not open LASlib lasreader");
    }
    ~las_reader ()
    {
        if (lasreader == nullptr)
            return;
        lasreader->close();
        delete lasreader;
    }
    LASreadOpener lasreadopener;
    LASreader *lasreader;
};

const std::string OGC_WKT_WARNING =
"WARNING: The LAS file's spatial coordinate system is either missing,\n"
"or it is not specified in OGC WKT format. This application can only\n"
"process spatial coordinate systems if they are specified in OGC WKT\n"
"format. To convert the LAS file's spatial coordinate system to OGC\n"
"WKT format, you can use LAStools.\n"
"\n"
"For example, you can use the LAStools 'las2las' command:\n"
"\n"
"    las2las -set_ogc_wkt -i input.las -o output.las\n"
"\n";

inline spoc::file::spoc_file read_las_file (const std::string &fn)
{
    using namespace spoc::file;
    using namespace spoc::header;
    using namespace spoc::point_record;

    // Try to read the las file
    las_reader l (fn);

    // Get the wkt
    std::string wkt;

    // Check the coordinate system
    if (l.lasreader->header.vlr_geo_ogc_wkt == nullptr)
        std::clog << OGC_WKT_WARNING << std::endl;
    else
        wkt = std::string (l.lasreader->header.vlr_geo_ogc_wkt);

    // Read the points
    const size_t total_points = l.lasreader->npoints;
    point_records prs;
    for (size_t i = 0; i < total_points; ++i)
    {
        if (!l.lasreader->read_point())
            throw std::runtime_error (std::string ("Error reading point record #") + std::to_string (i));

        point_record p;
        p.x = l.lasreader->point.get_x();
        p.y = l.lasreader->point.get_y();
        p.z = l.lasreader->point.get_z();
        p.c = l.lasreader->point.get_classification();
        p.p = l.lasreader->point.get_point_source_ID();
        p.i = l.lasreader->point.get_intensity();
        p.r = l.lasreader->point.rgb[0];
        p.g = l.lasreader->point.rgb[1];
        p.b = l.lasreader->point.rgb[2];
        prs.push_back (p);
    }

    const bool compressed = false;
    return spoc_file (wkt, compressed, prs);
}

inline spoc::file::spoc_file read_spoc_or_las_file (const std::string &fn)
{
    // Try to open the file
    std::ifstream ifs (fn);

    if (!ifs)
        throw std::runtime_error ("Could not open file for writing");

    // Try to read the spoc file
    try { return spoc::io::read_spoc_file (ifs); }
    catch (...) { ifs.close (); }

    // If we got this far, we have failed, so try to read
    // the specified filename as a LAS file instead
    return read_las_file (fn);
}


} // namespace spoc_conversion
