#include "enu.h"
#include <math.h>

void LatLonToENU(double lat, double lon, double h, double lat0, double lon0, double h0, double *enu_x, double *enu_y,
                 double *enu_z)
{
    double a, b, f, e_sq, pi;
    pi = 3.14159265359;
    a = 6378137;
    b = 6356752.3142;
    f = (a - b) / a;
    e_sq = f * (2 - f);
    // 站点（非原点）
    double lamb, phi, s, N, sin_lambda, cos_lambda, sin_phi, cos_phi, x, y, z;
    lamb = lat / 180.0 * pi;
    phi = lon / 180.0 * pi;
    s = sin(lamb);
    N = a / sqrt(1 - e_sq * s * s);

    sin_lambda = sin(lamb);
    cos_lambda = cos(lamb);
    sin_phi = sin(phi);
    cos_phi = cos(phi);

    x = (h + N) * cos_lambda * cos_phi;
    y = (h + N) * cos_lambda * sin_phi;
    z = (h + (1 - e_sq) * N) * sin_lambda;
    // 原点坐标转换
    double lamb0, phi0, s0, N0, sin_lambda0, cos_lambda0, sin_phi0, cos_phi0, x0, y0, z0;
    lamb0 = lat0 / 180.0 * pi;
    phi0 = lon0 / 180.0 * pi;
    s0 = sin(lamb0);
    N0 = a / sqrt(1 - e_sq * s0 * s0);

    sin_lambda0 = sin(lamb0);
    cos_lambda0 = cos(lamb0);
    sin_phi0 = sin(phi0);
    cos_phi0 = cos(phi0);

    x0 = (h0 + N0) * cos_lambda0 * cos_phi0;
    y0 = (h0 + N0) * cos_lambda0 * sin_phi0;
    z0 = (h0 + (1 - e_sq) * N0) * sin_lambda0;
    // ECEF 转 ENU
    double xd, yd, zd, t;
    xd = x - x0;
    yd = y - y0;
    zd = z - z0;
    t = -cos_phi0 * xd - sin_phi0 * yd;

    *enu_x = -sin_phi0 * xd + cos_phi0 * yd;
    *enu_y = t * sin_lambda0 + cos_lambda0 * zd;
    *enu_z = cos_lambda0 * cos_phi0 * xd + cos_lambda0 * sin_phi0 * yd + sin_lambda0 * zd;
}

void ENUToLatLon(double *lat, double *lon, double *h, double lat0, double lon0, double h0, double xEast, double yNorth,
                 double zUp)
{
    double a, b, f, e_sq, pi;
    pi = 3.14159265359;
    a = 6378137;
    b = 6356752.3142;
    f = (a - b) / a;
    e_sq = f * (2 - f);
    pi = 3.14159265359;
    double lamb, phi, s, N, sin_lambda, cos_lambda, sin_phi, cos_phi;
    lamb = pi / 180 * (lat0);
    phi = pi / 180 * (lon0);
    s = sin(lamb);
    N = a / sqrt(1 - e_sq * s * s);

    sin_lambda = sin(lamb);
    cos_lambda = cos(lamb);
    sin_phi = sin(phi);
    cos_phi = cos(phi);

    double x0, y0, z0, t, zd, xd, yd, x, y, z;
    x0 = (h0 + N) * cos_lambda * cos_phi;
    y0 = (h0 + N) * cos_lambda * sin_phi;
    z0 = (h0 + (1 - e_sq) * N) * sin_lambda;

    t = cos_lambda * zUp - sin_lambda * yNorth;

    zd = sin_lambda * zUp + cos_lambda * yNorth;
    xd = cos_phi * t - sin_phi * xEast;
    yd = sin_phi * t + cos_phi * xEast;

    x = xd + x0;
    y = yd + y0;
    z = zd + z0;

    double x2, y2, z2;
    x2 = x * x;
    y2 = y * y;
    z2 = z * z;

    double e, b2, e2, ep, r, r2, E2, F, G, c, s2, P, Q, ro, tmp, U, V, zo, height, temp;
    e = sqrt(1 - (b / a) * (b / a));
    b2 = b * b;
    e2 = e * e;
    ep = e * (a / b);
    r = sqrt(x2 + y2);
    r2 = r * r;
    E2 = a * a - b * b;
    F = 54 * b2 * z2;
    G = r2 + (1 - e2) * z2 - e2 * E2;
    c = (e2 * e2 * F * r2) / (G * G * G);
    s2 = pow((1 + c + sqrt(c * c + 2 * c)), (1 / 3));
    P = F / (3 * (s2 + 1 / s2 + 1) * (s2 + 1 / s2 + 1) * G * G);
    Q = sqrt(1 + 2 * e2 * e2 * P);
    ro = -(P * e2 * r) / (1 + Q) + sqrt((a * a / 2) * (1 + 1 / Q) - (P * (1 - e2) * z2) / (Q * (1 + Q)) - P * r2 / 2);
    tmp = (r - e2 * ro) * (r - e2 * ro);
    U = sqrt(tmp + z2);
    V = sqrt(tmp + (1 - e2) * z2);
    zo = (b2 * z) / (a * V);

    height = U * (1 - b2 / (a * V));

    *lat = atan((z + ep * ep * zo) / r);

    temp = atan(y / x);

    double longitude;
    if (x >= 0)
        longitude = temp;
    else {
        if ((x < 0) && (y >= 0))
            longitude = pi + temp;
        else
            longitude = temp - pi;
    }

    *lat = *lat / (pi / 180);
    *lon = longitude / (pi / 180);
    *h = height;
}