

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

} // namespace
