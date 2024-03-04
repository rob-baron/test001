#include "gis_radar.h"

#include <limits.h>
#include <math.h>
#include "gtest/gtest.h"

namespace
{

    typedef struct
    {
        location_str_t loc_str1;
        location_str_t loc_str2;
        radar_t radar;
    } test_g2r_data_t;

    const int num_g2r_tests = 28;
    test_g2r_data_t g2r_data[num_g2r_tests] = {
        {{"0N", "0E"}, {"0N", "1E"}, {111.20621865770366, 90}},
        {{"0N", "0E"}, {"1N", "1E"}, {157.28132136769725, 45}},
        {{"0N", "0E"}, {"1N", "0E"}, {111.20621865770366, 0}},
        {{"0N", "0E"}, {"1N", "1W"}, {157.28132136769725, 315}},
        {{"0N", "0E"}, {"0N", "1W"}, {111.20621865770366, 270}},
        {{"0N", "0E"}, {"1S", "1W"}, {157.28132136769725, 225}},
        {{"0N", "0E"}, {"1S", "0W"}, {111.20621865770366, 180}},
        {{"0N", "0E"}, {"1S", "1E"}, {157.28132136769725, 135}},

        {{"1N", "179E"}, {"1S", "179W"}, {314.7544702919983, 135}},
        {{"1S", "179W"}, {"1N", "179E"}, {314.7544702919983, 315}},
        {{"1S", "179E"}, {"1N", "179W"}, {314.7544702919983, 45}},
        {{"1N", "179W"}, {"1S", "179E"}, {314.7544702919983, 225}},

        /* North is the IDT
        ** South is the GM
        ** degrees go clockwise
        */
        {{"90N", "0E"}, {"89N", "180E"}, {111.20621865770366, 0}},
        {{"90N", "0E"}, {"89N", "135E"}, {111.20621865770366, 45}},
        {{"90N", "0E"}, {"89N", "90E"}, {111.20621865770366, 90}},
        {{"90N", "0E"}, {"89N", "45E"}, {111.20621865770366, 135}},
        {{"90N", "0E"}, {"89N", "0E"}, {111.20621865770366, 180}},
        {{"90N", "0E"}, {"89N", "45W"}, {111.20621865770366, 225}},
        {{"90N", "0E"}, {"89N", "90W"}, {111.20621865770366, 270}},
        {{"90N", "0E"}, {"89N", "135W"}, {111.20621865770366, 315}},

        /* defining north is GM
        ** south is IDL
        ** degrees go clockwise
        */
        {{"90S", "0E"}, {"89S", "180E"}, {111.20621865770366, 180}},
        {{"90S", "0E"}, {"89S", "135E"}, {111.20621865770366, 135}},
        {{"90S", "0E"}, {"89S", "90E"}, {111.20621865770366, 90}},
        {{"90S", "0E"}, {"89S", "45E"}, {111.20621865770366, 45}},
        {{"90S", "0E"}, {"89S", "0E"}, {111.20621865770366, 0}},
        {{"90S", "0E"}, {"89S", "45W"}, {111.20621865770366, 315}},
        {{"90S", "0E"}, {"89S", "90W"}, {111.20621865770366, 270}},
        {{"90S", "0E"}, {"89S", "135W"}, {111.20621865770366, 225}},
    };

    TEST(gis2radar, overall)
    {
        location_str_t locstr1, locstr2;
        radar_t radar;
        for (int i = 0; i < num_g2r_tests; i++)
        {
            gis2radar(&g2r_data[i].loc_str1, &g2r_data[i].loc_str2, &radar);
            EXPECT_EQ(0, cmpfp(radar.range, g2r_data[i].radar.range, 0.005)) << i << "  (range)\n"
                                                                             << "  { " << g2r_data[i].loc_str1.latitude << ", " << g2r_data[i].loc_str1.longitude << "} \n"
                                                                             << "  { " << g2r_data[i].loc_str2.latitude << ", " << g2r_data[i].loc_str2.longitude << "} \n"
                                                                             << "  { " << g2r_data[i].radar.range << ", " << g2r_data[i].radar.bearing << "} \n"
                                                                             << "  [ " << radar.range << ", " << radar.bearing << " ]\n"
                                                                             << std::endl;
            EXPECT_EQ(0, cmpfp(radar.bearing, g2r_data[i].radar.bearing, 0.005)) << i << "  (range)\n"
                                                                                 << "  { " << g2r_data[i].loc_str1.latitude << ", " << g2r_data[i].loc_str1.longitude << "} \n"
                                                                                 << "  { " << g2r_data[i].loc_str2.latitude << ", " << g2r_data[i].loc_str2.longitude << "} \n"
                                                                                 << "  { " << g2r_data[i].radar.range << ", " << g2r_data[i].radar.bearing << "} \n"
                                                                                 << "  [ " << radar.range << ", " << radar.bearing << " ]\n"
                                                                                 << std::endl;
        }
    }

    TEST(radar2gis, overall)
    {
        location_str_t locstr1, locstr2;
        radar_t radar;
        for (int i = 0; i < num_g2r_tests; i++)
        {
            radar2gis(&g2r_data[i].loc_str1, &g2r_data[i].radar, &locstr2);
            EXPECT_EQ(1, loc_str_isequal(&locstr2, &g2r_data[i].loc_str2)) << i << " location\n"
                                                                           << "  { " << g2r_data[i].loc_str1.latitude << ", " << g2r_data[i].loc_str1.longitude << "} \n"
                                                                           << "  { " << g2r_data[i].loc_str2.latitude << ", " << g2r_data[i].loc_str2.longitude << "} \n"
                                                                           << "  [ " << locstr2.latitude << ", " << locstr2.longitude << " ] \n"
                                                                           << std::endl;
        }
    }
} // namespace
