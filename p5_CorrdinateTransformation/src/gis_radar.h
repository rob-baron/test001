#ifndef __GIS_RADAR_H__
#define __GIS_RADAR_H__

#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5

#define ERROR_100 100
#define LATLON_STRLEN 100
#define EARTH_RADIUS 6371 // in km

// error_msg[1]="internal Error";
// error_msg[2]="input string does not match '^([0-9]+[.]?[0-9]*)([NSEW])$'";
// error_msg[3]="latitude is not 0 <= latitude <= 90"
// error_msg[5]="out of bounds"

// error_msg[100] = "System Error"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        double range;
        double bearing;
    } radar_t;

    typedef struct
    {
        double latitude;
        double longitude;
    } location_t;

    typedef struct
    {
        char latitude[LATLON_STRLEN];
        char longitude[LATLON_STRLEN];
    } location_str_t;

    typedef struct
    {
        double theta;
        double phi;
        double rho;
    } polar_t;

    int cvt_string(char *input, double *num, char *c);
    int force_range(double *value, double min, double max, double tollerance);

    /* converts latitude string ("10N") to a double () */
    int latstr2double(char *latstr, double *latdouble);

    /* converts lognitude string ("10W" to a double () */
    int lonstr2double(char *lonstr, double *londouble);

    int latdouble2str(double latdboule, char *latstr, unsigned long n);
    int londouble2str(double londouble, char *lonstr, unsigned long n);

    int loc_str2double(location_str_t *locstr, location_t *loc);
    int loc_double2str(location_t *loc, location_str_t *locstr);
    int location2polar(location_t *loc, double rho, polar_t *p);
    int location2polar(location_t *loc, double rho, polar_t *p);
    int vector_sub(double *vec1, double *vec2, double *result_vec, int n);
    int cmpfp(double a, double b, double tollerance);

    double deg2rad(double deg);
    double rad2deg(double rad);

    double vector_magnitude(double *vec, int n);

    /* given a location(lat/long)*/
    int gis2radar(location_str_t *loc1, location_str_t *loc2, radar_t *radar);
    int radar2gis(location_str_t *loc, radar_t *radar, location_str_t *loc2);

    double radians2degrees(double rad);
    double degrees2radians(double deg);

#ifdef __cplusplus
}
#endif

#endif //__GIS_RADAR_H__