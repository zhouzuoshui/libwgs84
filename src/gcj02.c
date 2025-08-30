#include <math.h>
#include <stdio.h>
#include "gcj02.h"

#define PI 3.14159265358979323
#define A 6378245.0
#define EE 0.00669342162296594323

int out_of_china(double lon, double lat)
{
    return !(lon > 73.66 && lon < 135.05 && lat > 3.86 && lat < 53.55);
}

double transform_lat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(fabs(x));
    ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * PI) + 40.0 * sin(y / 3.0 * PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * PI) + 320 * sin(y * PI / 30.0)) * 2.0 / 3.0;
    return ret;
}

double transform_lon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(fabs(x));
    ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * PI) + 40.0 * sin(x / 3.0 * PI)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * PI) + 300.0 * sin(x / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}

void WGS84ToGCJ02(double wg_lon, double wg_lat, double *gcj_lon, double *gcj_lat)
{
    if (out_of_china(wg_lon, wg_lat)) {
        *gcj_lon = wg_lon;
        *gcj_lat = wg_lat;
        return;
    }
    double dlat = transform_lat(wg_lon - 105.0, wg_lat - 35.0);
    double dlng = transform_lon(wg_lon - 105.0, wg_lat - 35.0);
    double radlat = wg_lat / 180.0 * PI;
    double magic = sin(radlat);
    magic = 1 - EE * magic * magic;
    double sqrtmagic = sqrt(magic);
    dlat = (dlat * 180.0) / ((A * (1 - EE)) / (magic * sqrtmagic) * PI);
    dlng = (dlng * 180.0) / (A / sqrtmagic * cos(radlat) * PI);
    *gcj_lat = wg_lat + dlat;
    *gcj_lon = wg_lon + dlng;
}

void GCJ02ToWGS84(double gcj_lon, double gcj_lat, double *wg_lon, double *wg_lat)
{
    if (out_of_china(gcj_lon, gcj_lat)) {
        *wg_lon = gcj_lon;
        *wg_lat = gcj_lat;
        return;
    }
    double dlat = transform_lat(gcj_lon - 105.0, gcj_lat - 35.0);
    double dlng = transform_lon(gcj_lon - 105.0, gcj_lat - 35.0);
    double radlat = gcj_lat / 180.0 * PI;
    double magic = sin(radlat);
    magic = 1 - EE * magic * magic;
    double sqrtmagic = sqrt(magic);
    dlat = (dlat * 180.0) / ((A * (1 - EE)) / (magic * sqrtmagic) * PI);
    dlng = (dlng * 180.0) / (A / sqrtmagic * cos(radlat) * PI);
    *wg_lat = gcj_lat - dlat;
    *wg_lon = gcj_lon - dlng;
}