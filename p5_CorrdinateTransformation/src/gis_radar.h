#ifndef __GIS_RADAR_H__
#define __GIS_RADAR_H__

#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5
#define ERROR_6 6

#define ERROR_100 100
#define LATLON_STRLEN 100
#define EARTH_RADIUS 6371 // in km
#define TOLERANCE 0.000000001

// error_msg[1]="internal Error";
// error_msg[2]="input string does not match '^([0-9]+[.]?[0-9]*)([NSEW])$'";
// error_msg[3]="latitude is not 0 <= latitude <= 90"
// error_msg[5]="out of bounds"
// error_msg[6]="NaN"
// error_msg[100] = "System Error"

#ifdef __cplusplus
extern "C"
{
#endif
    enum axis_t
    {
        XAXIS,
        YAXIS,
        ZAXIS
    };

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
    } spherical_t;

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

    int location2spherical(location_t *loc, double rho, spherical_t *sc);
    int spherical2locationl(spherical_t *sc, location_t *loc);

    int spherical2cartesian(spherical_t *sc, double *a);
    int cartesian2spherical(double *a, spherical_t *sc);
    int cmpfp(double a, double b, double tollerance);
    int vec_isequal(double *vec1, double *vec2, int n, double tollarance);

    double deg2rad(double deg);
    double rad2deg(double rad);

    int loc_isequal(location_t *loc1, location_t *loc2);
    int spherical_isequal(spherical_t *loc1, spherical_t *loc2);
    int radar_isequal(radar_t *rad1, radar_t *rad2);

    double vec_mag(double *vec, int n);
    int vec_sub(double *, double *, double *, int n);
    int matrix_vec_multiply(double *matrix, double *vec, int n, int m, double *vec_out);
    int rotate_vec(double *vec, double theata, enum axis_t axis, double *vec_rotated);

    /* given a location(lat/long)*/
    int gis2radar(location_str_t *loc1, location_str_t *loc2, radar_t *radar);
    int radar2gis(location_str_t *loc, radar_t *radar, location_str_t *loc2);

#ifdef __cplusplus
}
#endif

#endif //__GIS_RADAR_H__