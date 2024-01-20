

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

    TEST(cvt_string, correctness)
    {
        int ec = 0;
        double num;
        char ch;
        typedef struct
        {
            int ec;
            char string_data[50];
            double value;
            char ch;
        } test_data_t;

        test_data_t td[] = {
            {0, "10N", 10.0, 'N'},
            {0, "11.1234N", 11.1234, 'N'},
            {0, "12.34567891N", 12.34567891, 'N'},
            {0, "10S", 10.0, 'S'},
            {0, "11.1234S", 11.1234, 'S'},
            {0, "12.34567891S", 12.34567891, 'S'},
            {ERROR_2, " 12.0N ", 0, ' '},
            {ERROR_2, "12.0", 0, ' '},
        };

        int n = sizeof(td) / sizeof(test_data_t);
        for (int x = 0; x < n; x++)
        {
            ec = cvt_string(td[x].string_data, &num, &ch);
            ASSERT_EQ(ec, td[x].ec) << " x = " << x << std::endl;
            if (!ec)
            {
                ASSERT_EQ(num, td[x].value) << " x = " << x << std::endl;
                ASSERT_EQ(ch, td[x].ch) << " x = " << x << std::endl;
            }
        }

        return;
    }

    TEST(force_range, correctness)
    {
        double initial_num, num;
        initial_num = num = 20.001;
        int ec;

        /* test the upper bound */
        ec = force_range(&num, 0.000, 20.001, 0.001);
        ASSERT_EQ(ec, 0) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;
        ASSERT_EQ(num, 20.001) << "num = " << num << std::endl;

        num = 20.001;
        initial_num = num + 0.001 - 0.0001;
        ec = force_range(&num, 0.000, 20.001, 0.001);
        ASSERT_EQ(ec, 0) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;
        ASSERT_EQ(num, 20.001) << "num = " << num << std::endl;

        num = 20.001;
        initial_num = num = num + 0.001 + 0.0001;
        ec = force_range(&num, 0.000, 20.001, 0.001);
        ASSERT_EQ(ec, ERROR_5) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;

        /* test the lower bound */
        initial_num = num = 0.001;
        ec = force_range(&num, 0.001, 20.001, 0.001);
        ASSERT_EQ(ec, 0) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;
        ASSERT_EQ(num, 0.001) << "num = " << num << std::endl;

        num = 0.001;
        initial_num = num - 0.001 + 0.0001;
        ec = force_range(&num, 0.001, 20.001, 0.001);
        ASSERT_EQ(ec, 0) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;
        ASSERT_EQ(num, 0.001) << "num = " << num << std::endl;

        num = 0.001;
        initial_num = num = num - 0.001 - 0.0001;
        ec = force_range(&num, 0.001, 20.001, 0.001);
        ASSERT_EQ(ec, ERROR_5) << "ec = " << ec << "(" << initial_num << " , " << num << ")" << std::endl;
    }

    TEST(latstr2double, test_10N)
    {
        double latitude;
        int ec;

        ec = latstr2double("10N", &latitude);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(latitude, 80.000);
    }

    TEST(latstr2double, test_10S)
    {
        double latitude;
        int ec;

        ec = latstr2double("10S", &latitude);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(latitude, 100.000);
    }

    TEST(latdouble2str, test_10N)
    {
        char latstr[100];
        int ec;

        ec = latdouble2str(80, latstr, 100);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(0, strncmp(latstr, "10.000000N", 100)) << "latstr = " << latstr << std::endl;
    }

    TEST(latdouble2str, test_10S)
    {
        char latstr[100];
        int ec;

        ec = latdouble2str(100, latstr, 100);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(0, strncmp(latstr, "10.000000S", 100)) << "latstr = " << latstr << std::endl;
    }

    TEST(lonstr2double, test_10E)
    {
        double longitude;
        int ec;

        ec = lonstr2double("10E", &longitude);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(longitude, 170.0) << "langitude = " << longitude << std::endl;
    }

    TEST(longdouble2str, test_10E)
    {
        char lonstr[100];
        int ec;

        ec = londouble2str(170.0, lonstr, 100);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(0, strncmp(lonstr, "10.000000E", 100)) << "lonstr = " << lonstr << std::endl;
    }

    TEST(loc_str2double, test_10N10E)
    {
        location_str_t locstr = {
            "10N",
            "10E"};
        location_t loc;
        int ec;
        ec = loc_str2double(&locstr, &loc);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(loc.latitude, 80.0);
        ASSERT_EQ(loc.longitude, 170.0);
    }

    TEST(loc_double2str, test_10N10E)
    {
        location_t loc = {
            80.0,
            170.0};
        location_str_t locstr;
        int ec;
        ec = loc_double2str(&loc, &locstr);
        ASSERT_EQ(ec, 0) << "ec = " << ec << std::endl;
        ASSERT_EQ(0, strncmp(locstr.latitude, "10.000000N", 100)) << "lat = " << locstr.latitude << std::endl;
        ASSERT_EQ(0, strncmp(locstr.longitude, "10.000000E", 100)) << "lon = " << locstr.longitude << std::endl;
    }

    TEST(cmpfp, correctness)
    {
        ASSERT_EQ(0, cmpfp(2.1, 2.1, 0.001)) << "2.1, 2.1, 0.001" << std::endl;
        ASSERT_EQ(-1, cmpfp(2.1, 2.2, 0.001)) << "2.1, 2.2, 0.001" << std::endl;
        ASSERT_EQ(1, cmpfp(2.2, 2.1, 0.001)) << ".2, 2.1, 0.001" << std::endl;

        ASSERT_EQ(0, cmpfp(2.001, 2.0015, 0.001)) << "2.001, 2.0015, 0.001" << std::endl;
        ASSERT_EQ(-1, cmpfp(2.0005, 2.0026, 0.001)) << "2.00005, 2.0015, 0.001)" << std::endl;
        ASSERT_EQ(1, cmpfp(2.0026, 2.0005, 0.001)) << "" << std::endl;
    }

    TEST(rad2deg, correctness)
    {
        ASSERT_EQ(0, cmpfp(rad2deg(2 * M_PI), 360, 0.00001));
        ASSERT_EQ(0, cmpfp(rad2deg(M_PI), 180, 0.00001));
    }

    TEST(deg2rad, correctness)
    {
        ASSERT_EQ(0, cmpfp(deg2rad(360), 2 * M_PI, 0.00001));
        ASSERT_EQ(0, cmpfp(deg2rad(180), M_PI, 0.00001));
    }

    TEST(vec_isequal, correctness1)
    {
        double vec1[3] = {0.0, 0.0, 1.0000};
        double vec2[3] = {0.0, 0.0, 1.0000};
        double vec3[3] = {0.0, 1.0, 1.0000};
        ASSERT_EQ(1, vec_isequal(vec1, vec2, 3, 0.0001)) << "vec1,vec2"
                                                         << "[ " << vec1[0] << "," << vec1[1] << "," << vec1[2] << "]"
                                                         << "[ " << vec2[0] << "," << vec2[1] << "," << vec2[2] << "]" << std::endl;
        ASSERT_EQ(0, vec_isequal(vec2, vec3, 3, 0.0001)) << "vec2,vec3"
                                                         << "[ " << vec2[0] << "," << vec2[1] << "," << vec2[2] << "]"
                                                         << "[ " << vec3[0] << "," << vec3[1] << "," << vec3[2] << "]" << std::endl;
    }

    TEST(spherical2cartesian, 0_0_5)
    {
        spherical_t pc;
        double a[3];
        double cmp_vec[3];
        int ec;

        pc = {0, 0, 5};
        cmp_vec[0] = 0.0;
        cmp_vec[1] = 0.0;
        cmp_vec[2] = 5.0;
        ec = spherical2cartesian(&pc, a);
        ASSERT_EQ(0, ec);
        ASSERT_EQ(1, vec_isequal(a, cmp_vec, 3, 0.00001)) << "[ " << a[0] << ", " << a[1] << ", " << a[2] << " ]"
                                                          << "[ " << cmp_vec[0] << ", " << cmp_vec[1] << ", " << cmp_vec[2] << " ]"
                                                          << std::endl;
    }

    TEST(vec_sub, correctness)
    {
        double a[3] = {1.0, 2.0, 3.0};
        double b[3] = {0.1, 0.2, 0.3};
        double c[3] = {-1, -1, -1};
        double d[3] = {0.9, 1.8, 2.7};
        int ec;

        ec = vec_sub(a, b, c, 3);
        ASSERT_EQ(0, ec);
        ASSERT_EQ(1, vec_isequal(c, d, 3, 0.0000001)) << "[ " << c[0] << ", " << c[1] << ", " << c[2] << " ]"
                                                      << "[ " << d[0] << ", " << d[1] << ", " << d[2] << " ]"
                                                      << std::endl;
    }

    TEST(vec_magnitude, correctness)
    {
        double a[3] = {0.0, 0.0, 5.0};
        double mag;

        mag = vec_mag(a, 3);
        ASSERT_EQ(5, mag);
    }

    TEST(matrix_vec_multiply, identity_vec)
    {
        double matrix[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        double vec1[3] = {1, 2, 3};
        double vec2[3];
        matrix_vec_multiply(matrix, vec1, 3, 3, vec2);
        vec_isequal(vec1, vec2, 3, 0.000001);
    }

    TEST(radar_isequal, correctness)
    {
        radar_t rad1 = {5.0, 45.00},
                rad2 = {5.0, 45.00},
                rad3 = {4.0, 40.00};
        ASSERT_EQ(1, radar_isequal(&rad1, &rad2)) << "rad1 != rad2" << std::endl;
        ASSERT_EQ(0, radar_isequal(&rad1, &rad3)) << "rad1 == rad3" << std::endl;
    }

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

    TEST(gis2radar, ex0N0Eto1N0E_3)
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

    TEST(gis2radar, ex0N0Eto1N0E_4)
    {
        double loc_vec1[3] = {-1 * EARTH_RADIUS, 0.0, 0.0},
               loc_vec2[3] = {-6370.029665841369, 7.801036440746317e-13, 111.18928141193238},
               cmp_vec[3] = {-0.97033415863097616, -7.801036440746317e-13, -111.18928141193238},
               diff_vec[3];
        vec_sub(loc_vec1, loc_vec2, diff_vec, 3);
        ASSERT_EQ(1, vec_isequal(diff_vec, cmp_vec, 3, TOLERANCE)) << "vec sup"
                                                                   << "[ " << diff_vec[0] << ", " << diff_vec[1] << ", " << diff_vec[2] << " ]\n"
                                                                   << "[ " << cmp_vec[0] << ", " << cmp_vec[1] << ", " << cmp_vec[2] << " ]" << std::endl;
    }

    TEST(gis2radar, ex0N0Eto1N0E_5)
    {
        double vec[3] = {-0.97033415863097616, -7.801036440746317e-13, -111.18928141193238},
               magnitude;
        magnitude = vec_mag(vec, 3);
        ASSERT_EQ(0, cmpfp(111.1935153202798, magnitude, TOLERANCE)) << "magnitude: " << magnitude << std::endl;
    }

    TEST(gis2radar, ex0N0Eto1N0E_6)
    {
        double magnitude = 111.1935153202798,
               ang_diff;
        ang_diff = 2 * asin(magnitude / (2 * EARTH_RADIUS)); /* in radians */
        double range = tan(ang_diff) * EARTH_RADIUS;
        ASSERT_EQ(0, cmpfp(111.20621865767335, range, 0.0000001)) << "range: " << range << std::endl;
    }

    /*
       TEST(gis2radar, correctness)
       {
           location_str_t loc1 = {"1.0N", "0E"},
                          loc2 = {"0N, 0E"};
           radar_t radar, correct_radar = {111.0, 0.0};
           int ec;

           ec = gis2radar(&loc1, &loc2, &radar);
           ASSERT_EQ(1, radar_isequal(&radar, &correct_radar)) << "[ " << radar.bearing << ", " << radar.range << " ]" << std::endl;
       }
   */
} // namespace
