
# For debugging purposes: ./compile.sh; ./run.sh 0
#ILSVRC2012_val_00000001_504_376_RGB_50.265

input_yuv=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/pics/1/ILSVRC2012_val_00000001_504_376_RGB.yuv

QP=$1
recons_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS/1/ILSVRC2012_val_00000001_504_376_RGB_${QP}_1.yuv
bit_hevc=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265/1/ILSVRC2012_val_00000001_504_376_RGB_${QP}_1.265

out_file=null.txt

# For HEVC Normal
# echo 'HEVC TEST QP = ' $QP
# echo 'Input YUV: ' $input_yuv
# echo 'Recons HEVC: ' $recons_hevc
# echo 'Bit265 HEVC: ' $bit_hevc



# echo '------Calculate quality HEVC: '
# ./calc_quality $input_yuv $recons_hevc $bit_hevc $out_file


# For ffmpeg
recons_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-RECONS-ffmpeg/1/ILSVRC2012_val_00000001_504_376_RGB_${QP}.yuv
bit_ffmpeg=/media/h2amer/MULTICOM102/103_HA/MULTICOM103/set_yuv/Seq-265-ffmpeg/1/ILSVRC2012_val_00000001_504_376_RGB_${QP}.265

echo 'FFMPEG TEST QP= ' $QP
echo 'Input YUV: ' $input_yuv
echo 'Recons FFMPEG: ' $recons_ffmpeg
echo 'Bit265 FFMPEG: ' $bit_ffmpeg



echo '---------List YUV Files:'
ls -lah $recons_hevc
ls -lah $recons_ffmpeg

echo '--------List 265 files:'
ls -lah $bit_hevc
ls -lah $bit_ffmpeg

echo '------Calculate quality ffmpeg: '
./calc_quality $input_yuv $recons_ffmpeg $bit_ffmpeg $out_file

