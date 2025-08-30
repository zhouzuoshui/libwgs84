#ifndef LIBWGS84_ENU_H
#define LIBWGS84_ENU_H

// from https://gitee.com/sangzeyao/gps-tool
// LatLonToENU
// Converts a latitude/longitude/altitude pair to x, y, z coordinates in the ENU
// with respect to a given origin point (lat0, lon0, h0).
// Inputs:
//   lat - WGS84 Latitude of the point, in degree.
//   lon - WGS84 Longitude of the point, in degree.
//   h - WGS84 altitude of the point, in meter.
//   lat0 - WGS84 Latitude of the origin point, in degree.
//   lon0 - WGS84 Longitude of the origin point, in degree.
//   h0 - WGS84 altitude of the origin point, in meter.
// Outputs:
//   enu_x - The x coordinate (easting) of the computed point. (in meters)
//   enu_y - The y coordinate (northing) of the computed point. (in meters)
//   enu_z - The z coordinate (up) of the computed point. (in meters)
// Returns:
// The function does not return a value.
void LatLonToENU(double lat, double lon, double h, double lat0, double lon0, double h0, double *enu_x, double *enu_y,
                 double *enu_z);

// ENUToLatLon
// Converts x, y coordinates and zone in the ENU to a latitude/longitude/altitude pair
// with respect to a given origin point (lat0, lon0, h0).
// Inputs:
//   lat0 - WGS84 Latitude of the origin point, in degree.
//   lon0 - WGS84 Longitude of the origin point, in degree.
//   h0 - WGS84 altitude of the origin point, in meter.
//   xEast - The x coordinate (easting) of the point. (in meters)
//   yNorth - The y coordinate (northing) of the point. (in meters)
//   zUp - The z coordinate (up) of the point. (in meters)
// Outputs:
//   lat - WGS84 Latitude of the point, in degree.
//   lon - WGS84 Longitude of the point, in degree.
//   h - WGS84 altitude of the point, in meter.
// Returns:
// The function does not return a value.
void ENUToLatLon(double *lat, double *lon, double *h, double lat0, double lon0, double h0, double xEast, double yNorth,
                 double zUp);

#endif