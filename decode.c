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

int decode_h264(AVFormatContext* fmt_ctx)
{
	av_register_all();
	//寻找解码器
	AVCodec *videoCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	avformat_find_stream_info(fmt_ctx, NULL);
	int ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1,-1,NULL, 0);
	if(!ret)
	{
		perror("cannot find AVMEDIA_TYPE_VIDEO frame");
		return 0;
	}
	//fmt_ctx->streams->index = ret;
	AVCodecContext *avctx = fmt_ctx->streams[ret]->codec;
	return avcodec_open2(avctx, videoCodec, NULL);
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		perror("error : invalid param \n");
		return -1;
	}

	AVFormatContext* fmt_ctx;
    if (avformat_open_input(&fmt_ctx, argv[1], NULL, NULL) < 0)
    {
    	printf("open file: %s failed\n", argv[1]);
    	return -1;
    }
	//获取数据帧
	if(!decode_h264(fmt_ctx))
		perror("decode h264 video failed \n");
	else perror("decode h264 succeed\n");
}
