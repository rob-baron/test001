# Corrdinate Transformation

To convert from GIS to Radar and Radar to GIS is  in essence a corrdinate transformation from spherical corrdinates to polar corrdinates.

# Choice of interface
I chose to use Lat/Lon in the GIS structure as strings as you can use human readable strings to define the positiion from a map.  The radar structure is range and bearing.

# Internal transformations

Since Lat/Lon is basically spherical corrdinates, I first con convert this to spherical coordintates by having theata be defined from O degrees (at the north pole, to 180 degrees at the south pole, with the phi being defined at 0 at the International Date Line, 90 = 90E line to 180 at 0.0E, to 240 = 90W.

# implementation

I have developed this with a Test Drived Development mentailaity.  That is I have been writing tests using google test while writing the code to perform the transformations.  By having a well defined set of test

I've had to refresh my memory of linear algebra and had forgotten some minor details such as that physicists and mathamaticians switch the definitions of theta and phi.

I had forgotten that physicist and mathamticians use slightly different definitions.
