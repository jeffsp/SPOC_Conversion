% LAS2SPOC(1) SPOC User's Manual | Version 0.1
% spoc@spocfile.xyz
% December 25, 2021

# NAME

las2spoc - Convert a LAS format file to a SPOC format file

# USAGE

las2spoc [*options*] *filename.las* [*filename.spoc*]

# DESCRIPTION

Convert a LAS format file to a SPOC format file


# OPTIONS

\-\-help, -h
:   Get help

\-\-verbose, -v
:   Set verbose mode ON

\-\-nowarn, -w
:   Don't warn if the coordinate system is not in OGC WKT format

\-\-version, -e
:   Print version information and exit

\-\-preserve-timestamps, -t
:   Preserve LAS time stamps by multiplying the GPS time field by
    the scalar 1e9 and storing it in the extra[0] field.

# SEE ALSO

SPOC2LAS(1)
