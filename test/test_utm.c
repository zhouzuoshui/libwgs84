#include <stdio.h>
#include "wgs84.h"

int main() {
    double lon = 106.923;
    double lat = 26.588;
    double alt = 10.2;
    int zone = 40;

    double x = 691502.889840;
    double y = 2942242.487993;


    // double x, y;
    // LatLonToUTMXY(lat * 1, lon * 1, &x, &y, &zone);
    // printf("lat: %lf, lon: %lf, alt: %lf -> x: %lf, y: %lf, z: %d\n", lat, lon, alt, x, y, zone);
    UTMXYToLatLon(x, y, zone, 0, &lat, &lon);
    printf("x: %lf, y: %lf, zone: %d -> lat: %lf, lon: %lf\n", x, y, zone, lat, lon);
    return 0;
}