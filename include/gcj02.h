#ifndef LIBWGS84_GCJ02_H
#define LIBWGS84_GCJ02_H

// WGS84ToGCJ02
// Converts a WGS84 lon-lat pair to GCJ02 lon-lat pair.
// Inputs:
//   wg_lon - WGS84 Longitude of the point, in degree.
//   wg_lat - WGS84 Latitude of the point, in degree.
// Outputs:
//   gcj_lon - GCJ02 Longitude of the point, in degree.
//   gcj_lat - GCJ02 Latitude of the point, in degree.
// Returns:
// The function does not return a value.
void WGS84ToGCJ02(double wg_lon, double wg_lat, double *gcj_lon, double *gcj_lat);

// GCJ02ToWGS84
// Converts a GCJ02 lon-lat pair to WGS84 lon-lat pair.
// Inputs:
//   gcj_lon - GCJ02 Longitude of the point, in degree.
//   gcj_lat - GCJ02 Latitude of the point, in degree.
// Outputs:
//   wg_lon - WGS84 Longitude of the point, in degree.
//   wg_lat - WGS84 Latitude of the point, in degree.
// Returns:
// The function does not return a value.
void GCJ02ToWGS84(double gcj_lon, double gcj_lat, double *wg_lon, double *wg_lat);

#endif