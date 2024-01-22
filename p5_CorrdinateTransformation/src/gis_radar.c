
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <regex.h>
#include <string.h>
#include "gis_radar.h"

int cvt_string(char *input, double *num, char *ch)
{
    regex_t regex;
    unsigned long num_groups;
    regmatch_t *groups;
    if (regcomp(&regex, "^([0-9]+[.]?[0-9]*)([NSEW])$", REG_EXTENDED) != 0)
    {
        return ERROR_1;
    }
    num_groups = regex.re_nsub + 1;
    groups = malloc(num_groups * sizeof(regmatch_t));
    if (!groups)
    {
        return ERROR_100;
    }
    if (regexec(&regex, input, num_groups, groups, 0))
    {
        return ERROR_2;
    }
    if (num_groups > 1)
    {
        char buf[50];

        /* get the degress first */
        strncpy(buf, &input[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);
        buf[groups[1].rm_eo - groups[1].rm_so] = 0;
        *num = atof(buf);

        /* now set the direction */
        *ch = input[groups[2].rm_so];
    }
    if (groups)
        free(groups);
    return 0;
}

int force_range(double *value, double min, double max, double tollerance)
{
    if (max < min)
        return ERROR_1;
    if (min - tollerance > *value || *value > max + tollerance)
        return ERROR_5;
    if (*value > max)
        *value = max;
    if (*value < min)
        *value = min;
    return 0;
}

/* latitude
    90N     -> 0 degrees
    0 N/0S  -> 90 degrees
    90S     -> 180 degrees

*/
int latstr2double(char *latstr, double *latdouble)
{
    double latitude;
    char lat_ch;
    int ec = 0;

    ec = cvt_string(latstr, &latitude, &lat_ch);
    if (!ec)
    {
        ec = force_range(&latitude, 0.0, 90.0, TOLERANCE);
        if (ec == ERROR_5)
            ec = ERROR_3;
        if (lat_ch == 'N')
            *latdouble = 90.00 - latitude;
        else if (lat_ch == 'S')
            *latdouble = latitude + 90;
        else
            ec = ERROR_4;
        ec = force_range(latdouble, 0.0, 180.0, TOLERANCE);
    }
    return ec;
}

/* lognitude string 10E to double -
    180W  0  180E
     W Grenich E
*/

int lonstr2double(char *lonstr, double *londouble)
{
    double longitude;
    char lon_ch;
    int ec = 0;

    ec = cvt_string(lonstr, &longitude, &lon_ch);
    if (!ec)
    {
        ec = force_range(&longitude, 0.0, 180.0, TOLERANCE);
        if (ec == ERROR_5)
            ec = ERROR_3;
        if (lon_ch == 'E')
            *londouble = 180.0 - longitude;
        else if (lon_ch == 'W')
            *londouble = 180.0 + longitude;
        else
            ec = ERROR_4;
        ec = force_range(londouble, 0.0, 360.0, TOLERANCE);
    }
    return ec;
}

int latdouble2str(double latdouble, char *latstr, unsigned long n)
{
    int ec;
    double latitude;
    char lat_ch;
    ec = force_range(&latdouble, 0, 180.0, TOLERANCE);
    if (!ec)
    {
        if (latdouble < 90)
        {
            latitude = 90.0 - latdouble;
            lat_ch = 'N';
        }
        else
        {
            latitude = latdouble - 90.0;
            lat_ch = 'S';
        }
        ec = force_range(&latitude, 0, 90.0, TOLERANCE);
        if (ec == ERROR_5)
            ec = ERROR_100;
        else
            snprintf(latstr, n, "%2.6lf%c", latitude, lat_ch);
    }
    return ec;
}

int londouble2str(double londouble, char *lonstr, unsigned long n)
{
    int ec;
    double longitude;
    char lon_ch;
    ec = force_range(&londouble, 0, 360.0, TOLERANCE);
    if (!ec)
    {
        if (londouble < 180)
        {
            longitude = 180.0 - londouble;
            lon_ch = 'E';
        }
        else
        {
            longitude = londouble - 180.0;
            lon_ch = 'W';
        }
        ec = force_range(&longitude, 0, 180.0, TOLERANCE);
        if (ec == ERROR_5)
            ec = ERROR_100;
        else
            snprintf(lonstr, n, "%2.6lf%c", longitude, lon_ch);
    }
    return ec;
}

int loc_str2double(location_str_t *locstr, location_t *loc)
{
    int ec;
    ec = latstr2double(locstr->latitude, &(loc->latitude));
    ec = lonstr2double(locstr->longitude, &(loc->longitude));
    return ec;
}

int loc_double2str(location_t *loc, location_str_t *locstr)
{
    int ec = 0;
    ec = latdouble2str(loc->latitude, locstr->latitude, LATLON_STRLEN);
    ec = londouble2str(loc->longitude, locstr->longitude, LATLON_STRLEN);
    return ec;
}

int spherical2cartesian(spherical_t *pc, double *a)
{
    /* x=rho*sin(theta)cos(phi)
       y=rho*sin(theta)sin(phi)
       z=rho*cos(theta)
    */
    a[0] = pc->rho * sin(pc->theta) * cos(pc->phi);
    a[1] = pc->rho * sin(pc->theta) * sin(pc->phi);
    a[2] = pc->rho * cos(pc->theta);
    return 0;
}

int cartesian2spherical(double *a, spherical_t *pc)
{
    pc->theta = atan(a[1] / a[0]);
    pc->rho = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    pc->phi = acos(a[2] / pc->rho);
    return 0;
}

int location2spherical(location_t *loc, double rho, spherical_t *p)
{
    /* need some error checking to ensure
       0 <= theta <= 180 <=
       0 <= phi <= 360 <=
    */
    p->theta = deg2rad(loc->latitude);
    p->phi = deg2rad(loc->longitude);
    p->rho = rho;
    return 0;
}

int loc_isequal(location_t *loc1, location_t *loc2)
{
    if (cmpfp(loc1->latitude, loc2->latitude, TOLERANCE) ||
        cmpfp(loc1->longitude, loc2->longitude, TOLERANCE))
        return 0;
    return 1;
}

int vec_sub(double *vec1, double *vec2, double *result_vec, int n)
{
    for (int x = 0; x < n; x++)
    {
        result_vec[x] = vec1[x] - vec2[x];
    }
    return 0;
}

int matrix_vec_multiply(double *matrix, double *vec, int m, int n, double *vec_out)
{
    for (int i = 0; i < n; i++)
        vec_out[i] = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            vec_out[i] += matrix[i * m + j] * vec[j];
        }
    }
    return 0;
}

int rotate_vec(double *vec, double theta, enum axis_t axis, double *vec_rotated)
{
    double matrix[3 * 3] = {1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};
    switch (axis)
    {
    case XAXIS:
        matrix[4] = cos(theta);
        matrix[5] = -sin(theta);
        matrix[7] = sin(theta);
        matrix[8] = cos(theta);
        break;
    case YAXIS:
        matrix[0] = cos(theta);
        matrix[2] = sin(theta);
        matrix[6] = -sin(theta);
        matrix[8] = cos(theta);
        break;
    case ZAXIS:
        matrix[0] = cos(theta);
        matrix[1] = -sin(theta);
        matrix[3] = sin(theta);
        matrix[4] = cos(theta);
        break;
    }
    matrix_vec_multiply(matrix, vec, 3, 3, vec_rotated);
    return 0;
}

double vec_mag(double *vec, int n)
{
    double sum = 0;
    for (int x = 0; x < n; x++)
    {
        sum += vec[x] * vec[x];
    }
    return sqrt(sum);
}

double rad2deg(double rad)
{
    return rad * 180 / M_PI;
}

double deg2rad(double deg)
{
    return deg * M_PI / 180;
}

int cmpfp(double a, double b, double tolerance)
{
    if (fabs(a) < tolerance && fabs(b) < tolerance)
    {
        /* in this application assume that a and b are aero
        ** As this method is notably bad at comparing
        ** 0 with 0.
        */
        return 0;
    }
    double diff = fabs(a - b);
    double err = fmax(fabs(a), fabs(b)) * tolerance;
    if (diff < err)
        return 0;
    else
    {
        if (a > b)
            return 1;
        else
            return -1;
    }
}

int vec_isequal(double *vec1, double *vec2, int n, double tolerance)
{
    /*
    if (cmpfp(vec1[0], vec2[0], tolerance) == 0 &&
        cmpfp(vec1[1], vec2[1], tolerance) == 0 &&
        cmpfp(vec1[2], vec2[2], tolerance) == 0)
        return 1;
    return 0;
    */

    for (int x = 0; x < n; x++)
    {
        if (cmpfp(vec1[x], vec2[x], tolerance) != 0)
            return 0;
    }
    return 1;
}

int radar_isequal(radar_t *rad1, radar_t *rad2)
{
    if (cmpfp(rad1->bearing, rad2->bearing, TOLERANCE) == 0 &&
        cmpfp(rad1->range, rad2->range, TOLERANCE) == 0)
        return 1;
    return 0;
}

int spherical_isequal(spherical_t *pol_loc1, spherical_t *pol_loc2)
{
    if (cmpfp(pol_loc1->theta, pol_loc2->theta, TOLERANCE) == 0 &&
        cmpfp(pol_loc1->phi, pol_loc2->phi, TOLERANCE) == 0 &&
        cmpfp(pol_loc1->rho, pol_loc2->rho, TOLERANCE) == 0)
        return 1;
    return 0;
}

int gis2radar(location_str_t *locstr1, location_str_t *locstr2, radar_t *radar)
{
    location_t loc1, loc2, delta;
    loc_str2double(locstr1, &loc1);
    loc_str2double(locstr2, &loc2);
    /* first to find the angle between the 2 locations*/
    spherical_t loc_spherical1, loc_spherical2, loc_sphericalN;
    double loc_vec1[3], loc_vec2[3], ref_vec3[3], loc_vecN[3], loc_to_loc2[3], diff_mag, ang_diff;
    location2spherical(&loc1, EARTH_RADIUS, &loc_spherical1);
    location2spherical(&loc2, EARTH_RADIUS, &loc_spherical2);
    spherical2cartesian(&loc_spherical1, loc_vec1);
    spherical2cartesian(&loc_spherical2, loc_vec2);
    vec_sub(loc_vec1, loc_vec2, loc_to_loc2, 3);
    diff_mag = vec_mag(loc_to_loc2, 3);

    ang_diff = 2 * asin(diff_mag / (2 * EARTH_RADIUS)); /* in radians */
    radar->range = tan(ang_diff) * EARTH_RADIUS;

    /* find the due north vector of same angle difference (in radians) */
    /* need to increate rho to the plane of the radar coordinate */

    loc_sphericalN.theta = loc_spherical1.theta + ang_diff;
    loc_sphericalN.phi = loc_spherical1.phi;
    loc_sphericalN.rho = loc_spherical1.rho;

    spherical2cartesian(&loc_sphericalN, loc_vecN);
    double loc_toN[3];
    vec_sub(loc_vec1, loc_vecN, loc_toN, 3);

    return 0;
}

int radar2gis(location_str_t *loc_str, radar_t *radar, location_str_t *)
{
    /* find angular */
    spherical_t spherical_loc1 = {0.0, 0.0, EARTH_RADIUS},
                spherical_loc2;
    spherical_loc2.theta = tan(radar->range / EARTH_RADIUS);
    spherical_loc2.phi = radar->bearing;
    spherical_loc2.rho = EARTH_RADIUS;
    double vec_loc1[3], vec_loc2[3];
    spherical2cartesian(&spherical_loc1, vec_loc1);
    spherical2cartesian(&spherical_loc2, vec_loc2);

    location_t loc;
    loc_str2double(loc_str, &loc);
    spherical_t spherical_loc;
    location2spherical(&loc, EARTH_RADIUS, &spherical_loc);

    double vec_rotated1[3], vec_rotated2[3];
    rotate_vec(vec_loc2, loc.latitude, XAXIS, vec_rotated2);
    rotate_vec(vec_loc1, loc.latitude, XAXIS, vec_rotated1); /* here just to test */

#if 1
    /* this should work */
    rotate_vec(vec_loc2, loc.longitude, ZAXIS, vec_rotated2);
    rotate_vec(vec_loc1, loc.longitude, ZAXIS, vec_rotated1); /* here just to test */
#else
    /* but this is simpler */
    cartesian2spherical(vec_rotated2, &spherical_loc2);
    cartesian2spherical(vec_rotated1, &spherical_loc1);
    spherical_loc1.phi += loc.logitude;
    spherical_loc2.phi += loc.logitude;
#endif

    return 0;
}
