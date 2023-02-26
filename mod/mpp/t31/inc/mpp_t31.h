
#ifndef __mpp_t31_h__
#define __mpp_t31_h__

int mpp_t31_init(const char *snsname);
int mpp_t31_start_video(int streamid);
int mpp_t31_stop_video(int streamid);
void *mpp_t31_get_video(int streamid, void *pack, int *pack_count);
void mpp_t31_get_video_free(int streamid, void *p);

int mpp_t31_ctrl_osd(int grp, int id, int x, int y);
int mpp_t31_date_osd(int grp, int id, int w, int h, char *p);

int mpp_t31_init_audio();
int mpp_t31_get_audio(uint8_t **dat, uint64_t *pts);

#endif
