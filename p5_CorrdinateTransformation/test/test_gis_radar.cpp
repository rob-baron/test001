

#include "gis_radar.h"

#include <limits.h>
#include <math.h>
#include "gtest/gtest.h"

namespace
{

    typedef struct
    {
        location_t loc1;
        location_t loc2;
        radar_t rad;
    } gis2location_t;

    // location_t loc1, loc2;
    // loc_str2double(locstr1, &loc1);
    // loc_str2double(locstr2, &loc2);
    TEST(gis2radar, ex0N0Eto1N0E_1)
    {
        location_t loc1, loc2;
        location_str_t locstr1 = {"0N", "0E"},
                       locstr2 = {"1N", "0E"};
        loc_str2double(&locstr1, &loc1);
        loc_str2double(&locstr2, &loc2);
        location_t cmp_loc1 = {90.0, 180.0},
                   cmp_loc2 = {89.0, 180.0};
        ASSERT_EQ(1, loc_isequal(&loc1, &cmp_loc1)) << "loc 1 = [" << loc1.latitude << "," << loc1.longitude << "]" << std::endl;
        ASSERT_EQ(1, loc_isequal(&loc2, &cmp_loc2)) << "loc 2 = [" << loc2.latitude << "," << loc2.longitude << "]" << std::endl;
    }

    // /* first to find the angle between the 2 locations*/
    // spherical_t spherical_loc, spherical_loc1, spherical_loc2;
    // location2spherical(&loc1, EARTH_RADIUS, &spherical_loc);
    // location2spherical(&loc2, EARTH_RADIUS, &spherical_loc2);
    TEST(gis2radar, ex0N0Eto1N0E_2)
    {
        location_t loc1 = {90.0, 180.0},
                   loc2 = {89.0, 180.0};
        spherical_t loc_spherical1, loc_spherical2;
        location2spherical(&loc1, EARTH_RADIUS, &loc_spherical1);
        location2spherical(&loc2, EARTH_RADIUS, &loc_spherical2);
        // spherical coordinates in radians
        spherical_t cmp_loc_spherical1 = {M_PI / 2, M_PI, EARTH_RADIUS},
                    cmp_loc_spherical2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        ASSERT_EQ(1, spherical_isequal(&loc_spherical1, &cmp_loc_spherical1)) << "loc 1 = [" << loc_spherical1.theta << "," << loc_spherical1.phi << "," << loc_spherical1.rho << "]\n"
                                                                              << "cmp 1 = [" << cmp_loc_spherical1.theta << "," << cmp_loc_spherical1.phi << "," << cmp_loc_spherical1.rho << "]" << std::endl;
        ASSERT_EQ(1, spherical_isequal(&loc_spherical2, &cmp_loc_spherical2)) << "loc 2 = [" << loc_spherical2.theta << "," << loc_spherical2.phi << "," << loc_spherical2.rho << "]\n"
                                                                              << "cmp 2 = [" << cmp_loc_spherical2.theta << "," << cmp_loc_spherical2.phi << "," << cmp_loc_spherical2.rho << "]" << std::endl;
    }

    // /* rotate around z so that loc_spherical1.phi is at 180 */
    // memcpy(&spherical_loc, &spherical_loc1, sizeof(spherical_t));
    // if (spherical_loc.phi < 180)
    // {
    //     spherical_loc1.phi = spherical_loc1.phi + (180 - spherical_loc.phi);
    //     spherical_loc2.phi = spherical_loc2.phi + (180 - spherical_loc.phi);
    // }
    // else if (spherical_loc.phi > 180)
    // {
    //     spherical_loc1.phi = spherical_loc1.phi - (spherical_loc.phi - 180);
    //     spherical_loc2.phi = spherical_loc2.phi - (spherical_loc.phi - 180);
    // }
    TEST(gis2radar, ex0N0Eto10E_3)
    {
        spherical_t spherical_loc = {M_PI / 2, M_PI, EARTH_RADIUS},
                    spherical_loc1 = {M_PI / 2, M_PI, EARTH_RADIUS},
                    spherical_loc2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        if (spherical_loc.phi < M_PI)
        {
            spherical_loc1.phi = spherical_loc1.phi + (M_PI - spherical_loc.phi);
            spherical_loc2.phi = spherical_loc2.phi + (M_PI - spherical_loc.phi);
        }
        else if (spherical_loc.phi > M_PI)
        {
            spherical_loc1.phi = spherical_loc1.phi - (spherical_loc.phi - M_PI);
            spherical_loc2.phi = spherical_loc2.phi - (spherical_loc.phi - M_PI);
        }
        // spherical coordinates in radians
        spherical_t cmp_spherical_loc1 = {M_PI / 2, M_PI, EARTH_RADIUS},
                    cmp_spherical_loc2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        ASSERT_EQ(1, spherical_isequal(&spherical_loc1, &cmp_spherical_loc1)) << "loc 1 = [" << spherical_loc1.theta << "," << spherical_loc1.phi << "," << spherical_loc1.rho << "]\n"
                                                                              << "cmp 1 = [" << cmp_spherical_loc1.theta << "," << cmp_spherical_loc1.phi << "," << cmp_spherical_loc1.rho << "]" << std::endl;
        ASSERT_EQ(1, spherical_isequal(&spherical_loc2, &cmp_spherical_loc2)) << "loc 2 = [" << spherical_loc2.theta << "," << spherical_loc2.phi << "," << spherical_loc2.rho << "]\n"
                                                                              << "cmp 2 = [" << cmp_spherical_loc2.theta << "," << cmp_spherical_loc2.phi << "," << cmp_spherical_loc2.rho << "]" << std::endl;
    }

    // double vec_loc1[3], vec_loc2[3];
    // spherical2cartesian(&spherical_loc1, vec_loc1);
    // spherical2cartesian(&spherical_loc2, vec_loc2);
    TEST(gis2radar, ex0N0Eto1N0E_4)
    {
        spherical_t loc_spherical1 = {M_PI / 2, M_PI, EARTH_RADIUS},
                    loc_spherical2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        double loc_vec1[3] = {0, 0, 0}, loc_vec2[3] = {0, 0, 0};
        spherical2cartesian(&loc_spherical1, loc_vec1);
        spherical2cartesian(&loc_spherical2, loc_vec2);
        double cmp_vec1[3] = {-1 * EARTH_RADIUS, 0.0, 0.0},
               cmp_vec2[3] = {-6370.029665841369, 7.801036440746317e-13, 111.18928141193238};
        ASSERT_EQ(1, vec_isequal(loc_vec1, cmp_vec1, 3, TOLERANCE)) << "vec 1"
                                                                    << "[ " << loc_vec1[0] << ", " << loc_vec1[1] << ", " << loc_vec1[2] << " ]\n"
                                                                    << "[ " << cmp_vec1[0] << ", " << cmp_vec1[1] << ", " << cmp_vec1[2] << " ]" << std::endl;
        ASSERT_EQ(1, vec_isequal(loc_vec2, cmp_vec2, 3, TOLERANCE)) << "vec 2"
                                                                    << "[ " << loc_vec2[0] << "," << loc_vec2[1] << "," << loc_vec2[2] << "]\n"
                                                                    << "[ " << cmp_vec2[0] << "," << cmp_vec2[1] << "," << cmp_vec2[2] << "]" << std::endl;
    }

    // double vec_rotated1[3], vec_rotated2[3];
    // rotate_vec(vec_loc1, -loc.latitude, XAXIS, vec_rotated1);
    // rotate_vec(vec_loc2, -loc.latitude, XAXIS, vec_rotated2);
    TEST(gis2radar, ex0N0Eto1N0E_5)
    {
        spherical_t spherical_loc = {M_PI / 2, M_PI, EARTH_RADIUS};
        double vec_loc1[3] = {-1 * EARTH_RADIUS, 0.0, 0.0},
               vec_loc2[3] = {-6370.029665841369, 7.801036440746317e-13, 111.18928141193238};
        double vec_rotated1[3], vec_rotated2[3];
        rotate_vec(vec_loc1, spherical_loc.theta, YAXIS, vec_rotated1);
        rotate_vec(vec_loc2, spherical_loc.theta, YAXIS, vec_rotated2);
        double cmp_vec1[3] = {0.0, 0.0, EARTH_RADIUS},
               cmp_vec2[3] = {111.189281411932, 0, 6370.029665841369};
        ASSERT_EQ(1, vec_isequal(vec_rotated1, cmp_vec1, 3, TOLERANCE)) << "vec 1"
                                                                        << "[ " << vec_rotated1[0] << ", " << vec_rotated1[1] << ", " << vec_rotated1[2] << " ]\n"
                                                                        << "[ " << cmp_vec1[0] << ", " << cmp_vec1[1] << ", " << cmp_vec1[2] << " ]" << std::endl;
        ASSERT_EQ(1, vec_isequal(vec_rotated2, cmp_vec2, 3, TOLERANCE)) << "vec 2"
                                                                        << "[ " << vec_rotated2[0] << "," << vec_rotated2[1] << "," << vec_rotated2[2] << "]\n"
                                                                        << "[ " << cmp_vec2[0] << "," << cmp_vec2[1] << "," << cmp_vec2[2] << "]" << std::endl;
    }

    // cartesian2spherical(vec_rotated1, &spherical_loc1);
    // cartesian2spherical(vec_rotated2, &spherical_loc2);
    TEST(gis2radar, ex0N0Eto1N0E_6)
    {
        double vec_rotated1[3] = {0.0, 0.0, EARTH_RADIUS},
               vec_rotated2[3] = {111.189281411932, 0, 6370.029665841369};
        spherical_t spherical_loc1, spherical_loc2;
        cartesian2spherical(vec_rotated1, &spherical_loc1);
        cartesian2spherical(vec_rotated2, &spherical_loc2);
        spherical_t cmp_spherical_loc1 = {0.0, 0.0, EARTH_RADIUS},
                    cmp_spherical_loc2 = {0.017453292519947913, 0, EARTH_RADIUS};
        ASSERT_EQ(1, spherical_isequal(&spherical_loc1, &cmp_spherical_loc1)) << "loc 1 = [" << spherical_loc1.theta << "," << spherical_loc1.phi << "," << spherical_loc1.rho << "]\n"
                                                                              << "cmp 1 = [" << cmp_spherical_loc1.theta << "," << cmp_spherical_loc1.phi << "," << cmp_spherical_loc1.rho << "]" << std::endl;
        ASSERT_EQ(1, spherical_isequal(&spherical_loc2, &cmp_spherical_loc2)) << "loc 2 = [" << spherical_loc2.theta << "," << spherical_loc2.phi << "," << spherical_loc2.rho << "]\n"
                                                                              << "cmp 2 = [" << cmp_spherical_loc2.theta << "," << cmp_spherical_loc2.phi << "," << cmp_spherical_loc2.rho << "]" << std::endl;
    }

    // /* vec_loc1 should be at the north pole*/
    // radar->bearing = spherical_loc2.phi;
    // radar->range = tan(spherical_loc2.theta) * EARTH_RADIUS;
    TEST(gis2radar, ex0N0Eto1N0E_7)
    {
        spherical_t spherical_loc2 = {0.017453292519947913, 0, EARTH_RADIUS};
        radar_t radar;
        radar.bearing = rad2deg(spherical_loc2.phi);
        radar.range = tan(spherical_loc2.theta) * EARTH_RADIUS;
        ASSERT_EQ(0, cmpfp(radar.bearing, 0.0, 0.000001)) << "bearing = " << radar.bearing << std::endl;
        ASSERT_EQ(0, cmpfp(radar.range, 111.0, 0.000001)) << "range = " << radar.range << std::endl;
    }

} // namespace
