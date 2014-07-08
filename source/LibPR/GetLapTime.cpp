#pragma warning(disable: 4996)

#include <stdio.h>
#include "GetLapTime.h"

#if defined(_WIN32)||defined(_WIN64)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#if defined(_WIN32)||defined(_WIN64)
float showLaptimeWin(SYSTEMTIME* start, SYSTEMTIME* end)
{
	int	difH = end->wHour - start->wHour;
	int	difM = end->wMinute - start->wMinute;
	int	difS = end->wSecond - start->wSecond;
	int	difMS = end->wMilliseconds - start->wMilliseconds;
	int difTotalMS = (((difH*60 + difM)*60) + difS)*1000 + difMS;
	fprintf(stderr,"Lap: %d.%03d (sec)\n",difTotalMS/1000,difTotalMS%1000);
	return (float)difTotalMS/1000.0f;
}
#else
float showLaptimeUnix(struct timeval* start, struct timeval* end)
{
	printf("Lap: %.3f (sec)\n",(end->tv_sec-start->tv_sec)+(end->tv_usec-start->tv_usec)/1.0e+6);
	return (float)((end->tv_sec-start->tv_sec)+(end->tv_usec-start->tv_usec)/1.0e+6);
}
#endif


GETLAPTIME* NewGetLapTime()
{
	GETLAPTIME* data = new GETLAPTIME;
#if defined(_WIN32)||defined(_WIN64)
	data->start = new SYSTEMTIME;
	data->end = new SYSTEMTIME;
#else
	data->start = new struct timeval;
	data->end = new struct timeval;
#endif
	data->laptime = 0.0f;
	return data;
}

void DeleteGetLapTime(GETLAPTIME* data)
{
#if defined(_WIN32)||defined(_WIN64)
	delete (SYSTEMTIME*)data->start;
	delete (SYSTEMTIME*)data->end;
#else
	delete (struct timeval*)data->start;
	delete (struct timeval*)data->end;
#endif
	delete data;
}

void GetLocaStartTime(GETLAPTIME* data)
{
#if defined(_WIN32)||defined(_WIN64)
	GetLocalTime((SYSTEMTIME*)data->start);
#else
	gettimeofday((struct timeval*)data->start,NULL);
#endif
}

void GetLocalEndTime(GETLAPTIME* data)
{
#if defined(_WIN32)||defined(_WIN64)
	GetLocalTime((SYSTEMTIME*)data->end);
#else
	gettimeofday((struct timeval*)data->end,NULL);
#endif
}

void ShowLapTime(GETLAPTIME* data)
{
#if defined(_WIN32)||defined(_WIN64)
	data->laptime = showLaptimeWin((SYSTEMTIME*)data->start,(SYSTEMTIME*)data->end);
#else
	data->laptime = showLaptimeUnix((struct timeval*)data->start,(struct timeval*)data->end);
#endif
}

