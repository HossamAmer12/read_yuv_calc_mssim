
# For debugging purposes: ./compile.sh; ./run.sh 0

input_yuv=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/pics/1/ILSVRC2012_val_00000107_504_376_Y.yuv

QP=$1
recons_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS/1/ILSVRC2012_val_00000107_504_376_Y_${QP}_1.yuv
bit_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265/1/ILSVRC2012_val_00000107_504_376_Y_${QP}_1.265

out_file=null.txt

# For HEVC Normal
echo 'HEVC TEST QP = ' $QP
echo 'Input YUV: ' $input_yuv
echo 'Recons HEVC: ' $recons_hevc
echo 'Bit265 HEVC: ' $bit_hevc



# echo '------Calculate quality HEVC: '
# ./calc_quality $input_yuv $recons_hevc $bit_hevc $out_file
#./hevcesbrowser_console_linux -i $bit_hevc >> go.txt
#frame_size=$(grep '^0x*' go.txt) # lines start with
#echo $frame_size


# For ffmpeg
recons_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS-ffmpeg/1/ILSVRC2012_val_00000107_504_376_Y_${QP}.yuv
bit_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265-ffmpeg/1/ILSVRC2012_val_00000107_504_376_Y_${QP}.265

echo 'FFMPEG TEST QP= ' $QP
echo 'Input YUV: ' $input_yuv
echo 'Recons FFMPEG: ' $recons_ffmpeg
echo 'Bit265 FFMPEG: ' $bit_ffmpeg


# To 265 with hevc
# ffmpeg -loglevel panic -y -f rawvideo -pix_fmt yuv420p -s:v 504x336  -i $input_yuv -c:v hevc -crf $QP -f hevc -preset ultrafast $bit_ffmpeg

# To 265 with x265
time ffmpeg -y -f rawvideo -pix_fmt gray -s:v 504x376  -i $input_yuv -c:v libx265 -crf $QP -f hevc -preset ultrafast $bit_ffmpeg

echo "&&&&"

# with hevc
echo "ffmpeg -loglevel panic -y -f rawvideo -pix_fmt yuv420p -s:v 504x376  -i $input_yuv -c:v hevc -crf $QP -f hevc -preset ultrafast $bit_ffmpeg"


# with x265
#echo "ffmpeg -loglevel panic -y -f rawvideo -pix_fmt gray -s:v 504x368  -i $input_yuv -c:v libx265 -crf $QP -f hevc -preset ultrafast $bit_ffmpeg"

#./hevcesbrowser_console_linux -i $bit_ffmpeg >> go.txt
#frame_size=$(grep '^0x*' go.txt) # lines start with
#echo $frame_size
#rm go.txt

echo '**********************TO YUVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV'

# To yuv
# ffmpeg -loglevel panic -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg

# old command
# ffmpeg  -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg

ffmpeg  -y -i $bit_ffmpeg -s 504x376 -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg


echo '&&&&'
echo "ffmpeg -loglevel panic -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p -preset ultrafast $recons_ffmpeg"

echo '---------List YUV Files:'
ls -lah $recons_hevc
ls -lah $recons_ffmpeg

echo '--------List 265 files:'
ls -lah $bit_hevc
ls -lah $bit_ffmpeg

echo '------Calculate quality ffmpeg: '
./calc_quality $input_yuv $recons_ffmpeg $bit_ffmpeg $out_file




# echo '##################'
# echo 'ffmpeg straight from yuv' # this way has the same quality metrics as the ffmpeg up there

# time ffmpeg -loglevel panic -y -f rawvideo -vcodec rawvideo -s 504x336 -pix_fmt yuv420p -i $input_yuv -c:v libx265 -crf $QP -preset ultrafast $bit_ffmpeg
# echo "ffmpeg -loglevel panic -y -f rawvideo -vcodec rawvideo -s 504x336 -pix_fmt yuv420p -i $input_yuv -c:v libx265 -crf $QP -preset ultrafast $OUTPUT_DEC_FILE1"

# # To yuv
# ffmpeg -loglevel panic -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg

# echo '---------List YUV Files:'
# ls -lah $recons_hevc
# ls -lah $recons_ffmpeg

# echo '--------List 265 files:'
# ls -lah $bit_hevc
# ls -lah $bit_ffmpeg

# echo '------Calculate quality ffmpeg: '
# ./calc_quality $input_yuv $recons_ffmpeg $bit_ffmpeg $out_file



# # Delete everything
# rm $bit_ffmpeg
# rm $recons_ffmpeg
# rm $out_file
