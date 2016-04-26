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

int height, width;
enum AVPixelFormat pix_fmt;

int decode_h264(AVFormatContext* fmt_ctx)
{
	printf("here ok 0\n");
	//寻找解码器
	AVCodec *videoCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	avformat_find_stream_info(fmt_ctx, NULL);


	printf("here ok 2\n");
	int ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1,-1,NULL, 0);
	if(ret < 0)
	{
		perror("cannot find AVMEDIA_TYPE_VIDEO frame");
		return 0;
	}
	printf("here ok\n");
	//fmt_ctx->streams->index = ret;
	AVStream *avstrem = fmt_ctx->streams[ret];
	AVCodecContext avctx;
	//宽高和模式
	avctx = *(avstrem->codec);
	height = avctx.height;
	width = avctx.width;
	pix_fmt = avctx.pix_fmt;
	//打开解码器
	if(avcodec_open2(avstrem->codec, videoCodec, NULL) < 0)
	{
		perror("open decoder failed\n");
		return -1;
	}
	printf("ok 0\n");
	AVPacket *en_avpkt = malloc(sizeof(AVPacket));

	//写文件头
	//AVDictionary *opts = NULL;
	//av_dict_set(opts, "refcounted_frames",  "0", 0);
	//avformat_write_header(fmt_ctx, NULL);
	printf("ok 1\n");
	AVCodecContext* code_ctx = fmt_ctx->streams[ret]->codec;
	//?
	if (!(fmt_ctx->streams[ret]->codec->codec->capabilities & CODEC_CAP_DELAY))
	{
		perror("error : \n");
		return -1;
	}
	printf("ok 3\n");
	int got_frame = 0;
	AVFrame* frame = av_frame_alloc();
	//
	frame->format = 0;
	while(1)
	{
		en_avpkt->data = NULL;
		en_avpkt->size = 0;
		av_init_packet(en_avpkt);
		if(av_read_frame(fmt_ctx, en_avpkt) < 0)
		{
			perror("read frame data failed.\n");
			return -1;
		}
		//解码
		int encode_ret = avcodec_encode_video2 (fmt_ctx->streams[ret]->codec,
				en_avpkt, frame, &got_frame);

		//av_frame_free(NULL);
		if(!encode_ret || !got_frame)
		{
			perror("decode failed \n");
			return -1;
		}
		if(av_write_frame(fmt_ctx, &en_avpkt) < 0)
		{
			perror("write frame failed \n");
			return -1;
		}
		printf("decode 1 frame\n");
	}

		printf("here ok 1\n");
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		perror("error : invalid param \n");
		return -1;
	}
	av_register_all();
	AVFormatContext* fmt_ctx = NULL;
	printf("here ok 4\n");
	printf("%s\n", argv[1]);
    if (avformat_open_input(&fmt_ctx, argv[1], NULL, NULL) < 0)
    {
    	printf("open file: %s failed\n", argv[1]);
    	return -1;
    }
    height =
    printf("here ok 3\n");
	//获取数据帧
	if(decode_h264(fmt_ctx) < 0)
		perror("decode h264 video failed \n");
	else perror("decode h264 succeed\n");
}
