
#ifndef __GET_LAP_TIME_H__
#define __GET_LAP_TIME_H__

typedef struct _GET_LAP_TIME
{
	void* start;
	void* end;
	float laptime;
}
GETLAPTIME;

GETLAPTIME* NewGetLapTime();
void DeleteGetLapTime(GETLAPTIME* data);

void GetLocaStartTime(GETLAPTIME* data);
void GetLocalEndTime(GETLAPTIME* data);
void ShowLapTime(GETLAPTIME* data);

#endif

