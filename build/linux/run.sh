input_yuv=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/pics/1/ILSVRC2012_val_00001000_504_336_RGB.yuv

QP=$1
recons_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS/1/ILSVRC2012_val_00001000_504_336_RGB_${QP}_1.yuv
bit_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265/1/ILSVRC2012_val_00001000_504_336_RGB_${QP}_1.265

out_file=/dev/null

# For HEVC Normal
echo 'HEVC TEST QP = ' $QP
echo $input_yuv
echo $recons_yuv
echo $bit_hevc
#./compile.sh
./calc_quality $input_yuv $recons_hevc $bit_hevc $out_file
#./hevcesbrowser_console_linux -i $bit_hevc >> go.txt
#frame_size=$(grep '^0x*' go.txt) # lines start with
#echo $frame_size


# For ffmpeg
recons_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS-ffmpeg/1/ILSVRC2012_val_00001000_504_336_RGB_${QP}.yuv
bit_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265-ffmpeg/1/ILSVRC2012_val_00001000_504_336_RGB_${QP}.265

echo 'FFMPEG TEST QP= ' $QP
echo $input_yuv
echo $recons_ffmpeg
echo $bit_ffmpeg

# To 265
ffmpeg -loglevel panic -y -f rawvideo -pix_fmt yuv420p -s:v 504x336  -i $input_yuv -c:v hevc -crf $QP -f hevc -preset ultrafast $bit_ffmpeg

echo "&&&&"
echo "ffmpeg -loglevel panic -y -f rawvideo -pix_fmt yuv420p -s:v 504x336  -i $input_yuv -c:v hevc -crf $QP -f hevc -preset ultrafast $bit_ffmpeg"

#./hevcesbrowser_console_linux -i $bit_ffmpeg >> go.txt
#frame_size=$(grep '^0x*' go.txt) # lines start with
#echo $frame_size
#rm go.txt

# To yuv
#ffmpeg -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmppeg
ffmpeg -loglevel panic -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg

echo '&&&&'
echo "ffmpeg -loglevel panic -y -i $bit_ffmpeg -c:v rawvideo -pix_fmt yuv420p $recons_ffmpeg"

echo 'List YUV Files:'
ls -lah $recons_hevc
ls -lah $recons_ffmpeg

echo 'List 265 files:'
ls -lah $bit_hevc
ls -lah $bit_ffmpeg

echo 'Calculate quality ffmpeg: '
./calc_quality $input_yuv $recons_ffmpeg $bit_ffmpeg $out_file

# Delete everything
rm $bit_ffmpeg
rm $recons_ffmpeg
