#pragma once

#include "../types.h"
#include "../shm.h"
#include "../module.h"
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>    /* For mode constants */
#include <fcntl.h>       /* For O_* constants */
#include <sys/time.h>
#include <unistd.h>
#include <sys/shm.h>

namespace module
{

namespace shm
{
// shm
#define SHM_NAME "/ugv"

struct SharedDecision
{
	Decision_t s_decision;
	bool isValid;
	pthread_spinlock_t lock;
};

struct SharedMetaData
{
	MetaNavigation_t s_navi;
	MetaLaserHdl_t s_laserHdl;
	MetaCameraBW_t s_cameraBW[3];
	MetaCameraC_t s_cameraC;
	MetaAslPoints_t s_aslPts[2];
	MetaAslObjects_t s_aslObjs[2];
	MetaHokuyoPoints_t s_hokuyoPts[2];
	MetaHokuyoObjects_t s_hokuyoObjs[2];
	bool isValid[14];
	pthread_spinlock_t locks[14];
};

struct SharedRecoData
{
	RecoObstacle_t s_obstacle;
	RecoStopLine_t s_stopline;
	RecoTrafficSign_t s_ts;
	RecoTrafficLight_t s_tl;
	RecoIntersection_t s_intersection;
	RecoTrackLdAd_t s_trackLdAd;
	RecoTrackPts_t s_trackPts;
	RecoSpot_t s_spot;
	bool isValid[8];
	pthread_spinlock_t locks[8];
};

struct SharedMarkers
{
	MarkerNavi_t s_navi;
	MarkerIntersection_t s_intersection;
	MarkerTaillightImage_t s_taillightImage;
	MarkerObstacle_t s_obstacle;
	MarkerHokuyoObs_t s_hokuyoobs;
	pthread_spinlock_t locks[5];
};

struct SharedMemory
{
	struct SharedDecision shm_decision;
	struct SharedMetaData shm_metaData;
	struct SharedRecoData shm_recoData;
	struct SharedMarkers shm_markers;
};

class SharedObjectsImpl : public SharedObjects
{
protected:
	struct SharedMemory* m_addr;

public:

	bool GetDecision(Decision_t* decision) const;
	bool SetDecision(const Decision_t& decision);

	bool SetMetaData(const MetaData_t& data, int index = 0);
	bool GetMetaData(MetaData* data, int index = 0) const;

	bool SetRecoData(const RecoData_t& data);
	bool GetRecoData(RecoData_t* data) const;

	bool SetMarker(const MarkerData_t& data);
	bool GetMarker(MarkerData_t* data) const;
};

}
}
