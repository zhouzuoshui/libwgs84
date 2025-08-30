// UTM.h

// Original Javascript by Chuck Taylor
// Port to C++ by Alex Hajnal
//
//
// This is a simple port of the code on the Geographic/UTM Coordinate Converter (1) page from Javascript to C++.
// Using this you can easily convert between UTM and WGS84 (latitude and longitude).
// Accuracy seems to be around 50cm (I suspect rounding errors are limiting precision).
// This code is provided as-is and has been minimally tested; enjoy but use at your own risk!
// The license for UTM.cpp and UTM.h is the same as the original Javascript:
// "The C++ source code in UTM.cpp and UTM.h may be copied and reused without restriction."
//
// 1) http://home.hiwaay.net/~taylorc/toolbox/geography/geoutm.html

#ifndef LIBWGS84_UTM_H
#define LIBWGS84_UTM_H

// LatLonToUTMXY
// Converts a latitude/longitude pair to x, y coordinates and zone in the
// Universal Transverse Mercator projection.
// Inputs:
//   lat - Latitude of the point, in degree.
//   lon - Longitude of the point, in degree.
// Outputs:
//   x - The x coordinate (easting) of the computed point. (in meters)
//   y - The y coordinate (northing) of the computed point. (in meters)
//   zone - UTM zone to be used for calculating values for x and y.
//          The routine will determine the appropriate zone from the value of lon.
// Returns:
// The function does not return a value.
void LatLonToUTMXY(double lat, double lon, double *x, double *y, int *zone);

// UTMXYToLatLon
//
// Converts x and y coordinates in the Universal Transverse Mercator//   The UTM zone parameter should be in the range
// [1,60].

// projection to a latitude/longitude pair.
//
// Inputs:
// x - The easting of the point, in meters.
// y - The northing of the point, in meters.
// zone - The UTM zone in which the point lies.
// southhemi - True if the point is in the southern hemisphere;
//               false otherwise.
//
// Outputs:
// lat - The latitude of the point, in degree.
// lon - The longitude of the point, in degree.
//
// Returns:
// The function does not return a value.
void UTMXYToLatLon(double x, double y, int zone, int southhemi, double *lat, double *lon);

#endif