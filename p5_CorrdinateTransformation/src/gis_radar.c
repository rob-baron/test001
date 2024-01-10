
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
        ec = force_range(&latitude, 0.0, 90.0, .00000001);
        if (ec == ERROR_5)
            ec = ERROR_3;
        if (lat_ch == 'N')
            *latdouble = 90.00 - latitude;
        else if (lat_ch == 'S')
            *latdouble = latitude + 90;
        else
            ec = ERROR_4;
        ec = force_range(latdouble, 0.0, 180.0, .00000001);
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
        ec = force_range(&longitude, 0.0, 180.0, .00000001);
        if (ec == ERROR_5)
            ec = ERROR_3;
        if (lon_ch == 'E')
            *londouble = 180.0 - longitude;
        else if (lon_ch == 'W')
            *londouble = 180.0 + longitude;
        else
            ec = ERROR_4;
        ec = force_range(londouble, 0.0, 360.0, .00000001);
    }
    return ec;
}

int latdouble2str(double latdouble, char *latstr, unsigned long n)
{
    int ec;
    double latitude;
    char lat_ch;
    ec = force_range(&latdouble, 0, 180.0, .00000001);
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
        ec = force_range(&latitude, 0, 90.0, .00000001);
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
    ec = force_range(&londouble, 0, 360.0, .00000001);
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
        ec = force_range(&longitude, 0, 180.0, .00000001);
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

int polar2cartesian(polar_t *pc, double *a)
{
    /* x=rho*sin(theta)cos(phi)
       y=rho*sin(theta)sin(phi)
       z=rho*cos(theta)
    */
    a[1] = pc->rho * sin(pc->theta) * cos(pc->phi);
    a[2] = pc->rho * sin(pc->theta) * sin(pc->phi);
    a[3] = pc->rho * cos(pc->theta);
}

int location2polar(location_t *loc, double rho, polar_t *p)
{
    /* need some error checking to ensure
       0 <= theta <= 180
       0 <= phi <= 360
    */
    p->theta = deg2rad(loc->latitude);
    p->phi = deg2rad(loc->longitude);
    p->rho = rho;
}

int vector_sub(double *vec1, double *vec2, double *result_vec, int n)
{
    for (int x; x < n; x++)
    {
        result_vec[0] = vec1[0] - vec2[0];
    }
    return 1;
}

double vector_magnitude(double *vec, int n)
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

int cmpfp(double a, double b, double tollerance)
{
    double diff = fabs(a - b);
    double err = fmax(fabs(a), fabs(b)) * tollerance;
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

int gis2radar(location_str_t *locstr1, location_str_t *locstr2, radar_t *radar)
{
    location_t loc1, loc2, delta;
    loc_str2double(locstr1, &loc1);
    loc_str2double(locstr2, &loc2);
    /* first to find the angle between the 2 locations*/
    polar_t loc_polar1, loc_polar2, loc_polarN;
    double loc_vec1[3], loc_vec2[3], ref_vec3[3], loc_vecN[3], diff_vec[3], diff_mag, ang_diff;
    location2polar(&loc1, EARTH_RADIUS, &loc_polar1);
    location2polar(&loc2, EARTH_RADIUS, &loc_polar2);
    polar2cartesian(&loc_polar1, loc_vec1);
    polar2cartesian(&loc_polar2, loc_vec2);
    vector_sub(loc_vec1, loc_vec2, diff_vec, 3);
    diff_mag = vector_magnitude(diff_vec, 3);
    ang_diff = 2 * asin(diff_mag / (2 * EARTH_RADIUS)); /* in radians */
    radar->range = tan(ang_diff) * EARTH_RADIUS;

    /* find the due north vector of same angle difference */
    double ang_diff_deg = rad2deg(ang_diff);

    loc_polarN.theta = loc_polar1.theta + ang_diff_deg;
    loc_polarN.phi = loc_polar1.phi;
    loc_polarN.rho = loc_polar1.rho;

    polar2cartesian(&loc_polarN, loc_vecN);

    return 0;
}

int radar2gis(location_str_t *loc, radar_t *radar, location_str_t *)
{
    return 0;
}
