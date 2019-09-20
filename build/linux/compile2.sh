prefix=/usr
exec_prefix=${prefix}
includedir=${prefix}/local/include/opencv4/
libdir=${exec_prefix}/lib/

g++ ../../jpeg_tcm/main2.cpp -o seq_run -I $includedir -L $libdir -lopencv_calib3d -lopencv_imgproc \
	-lopencv_core -lopencv_ml -lopencv_features2d -lopencv_objdetect -lopencv_flann \
	-lopencv_video -lopencv_highgui
