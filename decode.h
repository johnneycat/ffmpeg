/*
 * decode.h
 *
 *  Created on: 2016年4月25日
 *      Author: wlw
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>

enum MEDIA_TYPE{
	VIDEO,
	AUDIO,
	UNKNOWN
};

//global variables
//输入&输出相关容器
static AVFormatContext *fmt_ctx = NULL;
//解码器ID及选用哪个解码器
static AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx = NULL;
//码流
static AVStream *video_stream = NULL, *audio_stream = NULL;
//存储输出解码输出数据（对于视频:YUV/RGB，对于音频：PCM）
static AVFrame *frame = NULL;
int fd_video;
int fd_audio;
AVCodec *videoCodec = NULL;


int decode_prepare(char* raw_file_path)
{
	char*video_dst_file = NULL;
	char*audio_dst_file = NULL;
	int n = strlen(raw_file_path);
	video_dst_file = malloc((n+4));
	audio_dst_file  = malloc((n+4));
	memcpy(video_dst_file, raw_file_path, n);
	memcpy(audio_dst_file, raw_file_path, n);
	strcat(video_dst_file, ".yuv420");
	strcat(audio_dst_file, ".pcm");
	fd_video = open(video_dst_file, O_RDWR | O_CREAT);
	fd_audio = open(audio_dst_file, O_RDWR | O_CREAT);
	if(fd_video < 0 || fd_audio < 0)
	{
		perror("create decode file failed!\n");
		return 0;
	}
	return 1;
}

int decoder_h264_init()
{
	//TODO：下面这句代码没有找到，但是网上都有的，可能是库版本不同
	//avcodec_init();
    avcodec_register_all();
    video_dec_ctx = avcodec_alloc_context();
    audio_dec_ctx = avcodec_alloc_context();
    //这里与网上的说法可能不同，
    videoCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if(!videoCodec)
    {
    	perror("error : cannot find decoder: %s\n", "AV_CODEC_ID_H264");
    	return 0;
    }
    return 1;
}

//解码一帧数据
int decode_to_h264(AVFormatContext *fmt_ctx)
{
	int ret, stream_index;
	AVStream *st;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;
	//TODO:type应该设置为视频
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if( ret < 0 )
    {
    	perror("cannot find any stream fit type\n");
    	return 0;
    }
    stream_index = ret;
    st = fmt_ctx->streams[stream_index];
    dec_ctx = st->codec;

}

void decode_h264(char* raw_file_path)
{
	if(!decode_prepare(raw_file_path))
		return;
	//解码器初始化
	if(!decoder_h264_init())
		return;

	//从video数据中取帧

	//真正的解码工作

}


int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("error : Invalid param\n");
		printf("usage: %s %s\n", "program name", "raw media name");
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("open file : %s failed\n", argv[1]);
		return -1;
	}
	//交给ffmpeg库来解决
    av_register_all();//初始化 libavformat和注册所有的muxers、demuxers和protocols
    if (avformat_open_input(&fmt_ctx, argv[1], NULL, NULL) < 0)
    {
    	perror("error : file name not matches file contents\n");
    	return -1;
    }

    //读取文件的一小部分结构，来初始化AVStream结构体(该结构体确定了输入输出的码流)
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
    	perror("error : could not find stream info - this file maybe not complete\n");
    	return -1;
    }
    decode_h264(argv[1]);
}
