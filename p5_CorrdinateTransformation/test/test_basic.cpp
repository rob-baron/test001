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

#if 0
    TEST(addrelativelocation, correctness)
    {
        typedef struct
        {
            location_str_t loc;
            location_str_t rel_loc;
            location_str_t summed_loc;
        } summed_location_test_t;

        const int num_rel_tests = 32;
        summed_location_test_t loc_data[num_rel_tests] = {
            {{"0N", "0E"}, {"1N", "OE"}, {"1N", "0E"}},
            {{"0N", "0E"}, {"1N", "1E"}, {"1N", "1E"}},
            {{"0N", "0E"}, {"0N", "1E"}, {"0N", "1E"}},
            {{"0N", "0E"}, {"1S", "1E"}, {"1S",""}},
            {{"0N", "0E"}, {"1S", "OE"}, {"1S",""}},
            {{"0N", "0E"}, {"1S", "1W"}, {"1S",""}},
            {{"0N", "0E"}, {"0N", "1E"}, {"0N",""}},
            {{"0N", "0E"}, {"1N", "1W"}, {"1N",""}},

            {{"0N", "180E"}, {"1N", "OE"}, {"1N",""}},
            {{"0N", "180E"}, {"1N", "1E"}, {"1N",""}},
            {{"0N", "180E"}, {"0N", "1E"}, {"0N",""}},
            {{"0N", "180E"}, {"1S", "1E"}, {"1S",""}},
            {{"0N", "180E"}, {"1S", "OE"}, {"1S",""}},
            {{"0N", "180E"}, {"1S", "1W"}, {"1S",""}},
            {{"0N", "180E"}, {"0N", "1E"}, {"0N",""}},
            {{"0N", "180E"}, {"1N", "1W"}, {"1N",""}},

            {{"90N", "0E"}, {"1N", "OE"}, {"89N","180E"}},
            {{"90N", "0E"}, {"1N", "1E"}, {"89N","135E"}},
            {{"90N", "0E"}, {"0N", "1E"}, {"89N","90E"}},
            {{"90N", "0E"}, {"1S", "1E"}, {"89N","45E"}},
            {{"90N", "0E"}, {"1S", "OE"}, {"89N","0E"}},
            {{"90N", "0E"}, {"1S", "1W"}, {"89N","45W"}},
            {{"90N", "0E"}, {"0N", "1E"}, {"89N","90W"}},
            {{"90N", "0E"}, {"1N", "1W"}, {"89N","135W"}},

            {{"90S", "0E"}, {"1N", "OE"}, {"89S","180E"}},
            {{"90S", "0E"}, {"1N", "1E"}, {"89S","135E"}},
            {{"90S", "0E"}, {"0N", "1E"}, {"89S","90E"}},
            {{"90S", "0E"}, {"1S", "1E"}, {"89S","45E"}},
            {{"90S", "0E"}, {"1S", "OE"}, {"89S","0E"}},
            {{"90S", "0E"}, {"1S", "1W"}, {"89S","45W"}},
            {{"90S", "0E"}, {"0N", "1E"}, {"89S","90W"}},
            {{"90S", "0E"}, {"1N", "1W"}, {"89S","135W"}},

        };

        location_str_t loc_str2;
        for (int j = 0; j < num_rel_tests; j++)
        {
            add_rel_position(&loc_data[i].loc, &loc_data[j].rel_loc, &loc_str2);
            ASSERT_EQ(1, loc_str2, loc_data[i].summed_loc) << i << "\n"
                                                           << "  { " << loc_data[i].loc.latitude << ", " << loc_data[i].loc.longitude << "} \n"
                                                           << "  { " << loc_data[i].rel_loc.latitude << ", " << loc_data[i].rel_loc.longitude << "} \n"
                                                           << "  { " << loc_data[i].summed_loc.latitude << ", " << loc_data[i].summed_loc.longitude << "} \n"
                                                           << std::endl;
        }
    }
#endif

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
        spherical_t sc;
        double a[3];
        double cmp_vec[3];
        int ec;

        sc = {0, 0, 5};
        cmp_vec[0] = 0.0;
        cmp_vec[1] = 0.0;
        cmp_vec[2] = 5.0;
        ec = spherical2cartesian(&sc, a);
        ASSERT_EQ(0, ec);
        ASSERT_EQ(1, vec_isequal(a, cmp_vec, 3, 0.00001)) << "[ " << a[0] << ", " << a[1] << ", " << a[2] << " ]"
                                                          << "[ " << cmp_vec[0] << ", " << cmp_vec[1] << ", " << cmp_vec[2] << " ]"
                                                          << std::endl;
    }

    TEST(cartesian2spherical, correctnessr)
    {
        spherical_t sc,
            sc_cmp,
            sc_degree[13] = {
                {0, 0, 5},
                {10, 0, 5},
                {20, 0, 5},
                {45, 0, 5},
                {90, 0, 5},
                {90, 45, 5},
                {90, 90, 5},
                {90, 135, 5},
                {90, 180, 5},
                {90, 225, 5},
                {90, 270, 5},
                {90, 315, 5},
                {180, 0, 5}};
        double a[3];
        for (int x = 0; x < 12; x++)
        {
            sc_cmp.theta = deg2rad(sc_degree[x].theta);
            sc_cmp.phi = deg2rad(sc_degree[x].phi);
            sc_cmp.r = deg2rad(sc_degree[x].r);
            spherical2cartesian(&sc_cmp, a);
            cartesian2spherical(a, &sc);
            ASSERT_EQ(1, spherical_isequal(&sc, &sc_cmp)) << "x=" << x
                                                          << "\n[ " << sc_degree[x].theta << ", " << sc_degree[x].phi << ", " << sc_degree[x].r << " ]"
                                                          << "\n[ " << sc_cmp.theta << ", " << sc_cmp.phi << ", " << sc_cmp.r << " ]"
                                                          << "\n[ " << a[0] << ", " << a[1] << ", " << a[2] << " ]"
                                                          << "\n[ " << sc.theta << ", " << sc.phi << ", " << sc.r << " ]"
                                                          << std::endl;
        }
    }

    TEST(cartesian2spherical, 10_20_5)
    {
        spherical_t sc, sc_cmp = {0.0, 0.0, 5};
        double a[3] = {0.0, 0.0, 5.0};

        cartesian2spherical(a, &sc);
        ASSERT_EQ(1, spherical_isequal(&sc, &sc_cmp)) << "[ " << sc.theta << ", " << sc.phi << ", " << sc.r << " ]"
                                                      << "[ " << sc_cmp.theta << ", " << sc_cmp.phi << ", " << sc_cmp.r << " ]"
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
        ASSERT_EQ(1, vec_isequal(vec1, vec2, 3, 0.000001)) << "[ " << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << " ]"
                                                           << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << std::endl;
    }

    TEST(matrix_vec_multiply, zrot90_vec)
    {
        double matrix[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
        double vec1[3] = {1, 0, 0};
        double vec2[3];
        double vec3[3] = {0, 1, 0};
        matrix_vec_multiply(matrix, vec1, 3, 3, vec2);
        ASSERT_EQ(1, vec_isequal(vec2, vec3, 3, 0.000001)) << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << "[ " << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << " ]"
                                                           << std::endl;
    }

    TEST(matrix_vec_multiply, matrix_vec)
    {
        double matrix[9] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
        double vec1[3] = {1, 2, 3};
        double vec2[3], vec3[3] = {6, 12, 18};
        matrix_vec_multiply(matrix, vec1, 3, 3, vec2);
        ASSERT_EQ(1, vec_isequal(vec2, vec3, 3, 0.000001)) << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << "[ " << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << " ]"
                                                           << std::endl;
    }

    TEST(rotate_vec, xaxis90)
    {
        double vec1[3] = {0, 0, 1};
        double vec2[3];
        double vec3[3] = {0, -1, 0};

        rotate_vec(vec1, deg2rad(90), XAXIS, vec2);
        ASSERT_EQ(1, vec_isequal(vec2, vec3, 3, 0.000001)) << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << "[ " << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << " ]"
                                                           << std::endl;
    }
    TEST(rotate_vec, yaxis90)
    {
        double vec1[3] = {0, 0, 1};
        double vec2[3];
        double vec3[3] = {1, 0, 0};
        rotate_vec(vec1, deg2rad(90), YAXIS, vec2);
        ASSERT_EQ(1, vec_isequal(vec2, vec3, 3, 0.000001)) << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << "[ " << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << " ]"
                                                           << std::endl;
    }
    TEST(rotate_vector, zaxis90)
    {
        double vec1[3] = {1, 0, 0};
        double vec2[3];
        double vec3[3] = {0, 1, 0};
        rotate_vec(vec1, deg2rad(90), ZAXIS, vec2);
        ASSERT_EQ(1, vec_isequal(vec2, vec3, 3, 0.000001)) << "[ " << vec2[0] << ", " << vec2[1] << ", " << vec2[2] << " ]"
                                                           << "[ " << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << " ]"
                                                           << std::endl;
    }

    TEST(radar_isequal, correctness)
    {
        radar_t rad1 = {5.0, 45.00},
                rad2 = {5.0, 45.00},
                rad3 = {4.0, 40.00};
        ASSERT_EQ(1, radar_isequal(&rad1, &rad2)) << "rad1 != rad2" << std::endl;
        ASSERT_EQ(0, radar_isequal(&rad1, &rad3)) << "rad1 == rad3" << std::endl;
    }

} // namespace
