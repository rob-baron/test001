

#include "gis_radar.h"

#include <limits.h>
#include <math.h>
#include "gtest/gtest.h"

namespace
{

    TEST(radar2gis, ex0N0Eto1N0E_1)
    {
        location_str_t locstr1 = {"0N", "0E"};
        location_t loc1;
        spherical_t sc_loc1;
        loc_str2double(&locstr1, &loc1);
        location2spherical(&loc1, EARTH_RADIUS, &sc_loc1);
        spherical_t cmp_sc_loc1 = {M_PI / 2, M_PI, EARTH_RADIUS};
        ASSERT_EQ(1, spherical_isequal(&sc_loc1, &cmp_sc_loc1)) << "loc 1 = [" << sc_loc1.theta << "," << sc_loc1.phi << "," << sc_loc1.r << "]\n"
                                                                << "cmp 1 = [" << cmp_sc_loc1.theta << "," << cmp_sc_loc1.phi << "," << cmp_sc_loc1.r << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_2)
    {
        /* layout the radar range/distance on top of the spherical coordinates */
        spherical_t spherical_loc2 = {0.0, 0.0, EARTH_RADIUS};
        radar_t radar = {111.20621865770366, 0.0};
        spherical_loc2.theta = atan(radar.range / EARTH_RADIUS);
        spherical_loc2.phi = radar.bearing;
        spherical_loc2.r = EARTH_RADIUS;
        spherical_t sc_cmp = {0.017453292519947913, 0, EARTH_RADIUS};
        ASSERT_EQ(1, spherical_isequal(&spherical_loc2, &sc_cmp)) << "loc 2 = [" << spherical_loc2.theta << "," << spherical_loc2.phi << "," << spherical_loc2.r << "]\n"
                                                                  << "cmp 2 = [" << sc_cmp.theta << "," << sc_cmp.phi << "," << sc_cmp.r << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_3)
    {
        spherical_t sc_loc2 = {0.017453292519947913, 0, EARTH_RADIUS};
        double vec_loc2[3],
            cmp_vec_loc2[3] = {111.189281411932, 0, 6370.029665841369};
        spherical2cartesian(&sc_loc2, vec_loc2);
        ASSERT_EQ(1, vec_isequal(vec_loc2, cmp_vec_loc2, 3, TOLERANCE)) << "vec 2\n"
                                                                        << "[ " << vec_loc2[0] << "," << vec_loc2[1] << "," << vec_loc2[2] << "]\n"
                                                                        << "[ " << cmp_vec_loc2[0] << "," << cmp_vec_loc2[1] << "," << cmp_vec_loc2[2] << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_4)
    {
        spherical_t sc_loc1 = {M_PI / 2, M_PI, EARTH_RADIUS};
        double vec_loc2[3] = {111.189281411932, 0, 6370.029665841369},
               cmp_vec_loc2[3] = {-6370.029665841369, 0, 111.18928141193238};
        double vec_rotated2[3];
        rotate_vec(vec_loc2, -sc_loc1.theta, YAXIS, vec_rotated2);
        ASSERT_EQ(1, vec_isequal(vec_rotated2, cmp_vec_loc2, 3, TOLERANCE)) << "vec 2\n"
                                                                            << "[ " << vec_rotated2[0] << "," << vec_rotated2[1] << "," << vec_rotated2[2] << "]\n"
                                                                            << "[ " << cmp_vec_loc2[0] << "," << cmp_vec_loc2[1] << "," << cmp_vec_loc2[2] << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_5)
    {
        double vec_loc2[3] = {-6370.029665841369, 0, 111.18928141193238};
        spherical_t sc_loc2,
            sc_loc2_cmp = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        cartesian2spherical(vec_loc2, &sc_loc2);
        ASSERT_EQ(1, spherical_isequal(&sc_loc2, &sc_loc2_cmp)) << "loc 2 = [" << sc_loc2.theta << "," << sc_loc2.phi << "," << sc_loc2.r << "]\n"
                                                                << "cmp 2 = [" << sc_loc2_cmp.theta << "," << sc_loc2_cmp.phi << "," << sc_loc2_cmp.r << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_6)
    {
        spherical_t sc_loc = {M_PI / 2, M_PI, EARTH_RADIUS},
                    sc_loc2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS},
                    sc_loc2_cmp = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        if (sc_loc.phi < M_PI)
        {
            sc_loc2.phi = sc_loc2.phi - (M_PI - sc_loc.phi);
        }
        else if (sc_loc.phi > M_PI)
        {
            sc_loc2.phi = sc_loc2.phi + (sc_loc.phi - M_PI);
        }
        ASSERT_EQ(1, spherical_isequal(&sc_loc2, &sc_loc2_cmp)) << "loc 2 = [" << sc_loc2.theta << "," << sc_loc2.phi << "," << sc_loc2.r << "]\n"
                                                                << "cmp 2 = [" << sc_loc2_cmp.theta << "," << sc_loc2_cmp.phi << "," << sc_loc2_cmp.r << "]" << std::endl;
    }

    TEST(radar2gis, ex0N0Eto1N0E_7)
    {
        spherical_t sc_loc2 = {89.0 * M_PI / 180.0, M_PI, EARTH_RADIUS};
        location_t loc2, cmp_loc2 = {89.0, 180.0};
        spherical2location(&sc_loc2, &loc2);
        ASSERT_EQ(1, loc_isequal(&loc2, &cmp_loc2)) << "loc 2 = [" << loc2.latitude << "," << loc2.longitude << "]"
                                                    << "cmp   = [" << cmp_loc2.latitude << "," << cmp_loc2.longitude << "]" << std::endl;
    }

    // location_str_t loc_str1;
    // double2loc_str(&loc1, &loc_str1); /* this should equal loc_str */
    // double2loc_str(&loc2, loc_str2);  /* output */
    TEST(radar2gis, ex0N0Eto1N0E_8)
    {
        location_t loc2 = {89.0, 180.0};
        location_str_t loc_str2, cmp_loc_str2 = {"1N", "0E"};
        loc_double2str(&loc2, &loc_str2);
        ASSERT_EQ(1, loc_str_isequal(&loc_str2, &cmp_loc_str2)) << "loc1 = [ " << loc_str2.latitude << "," << loc_str2.longitude << " ]\n"
                                                                << "loc2 = [ " << cmp_loc_str2.latitude << "," << cmp_loc_str2.longitude << " ]\n"
                                                                << std::endl;
    }
} // namespace
