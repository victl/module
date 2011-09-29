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

struct SharedConfig
{
	Config_t s_config;
	bool isValid;
	pthread_spinlock_t lock;
};

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
	bool isNew[14];
	struct timeval timestamps[14];
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
	RecoTrackRoad_t s_trackRoad;
	RecoSpot_t s_spot;
	RecoSpotLine_t s_spotline;
	RecoSideParking_t s_sideParking;
	bool isNew[RecoData::RT_MAX - 1];
	bool isValid[RecoData::RT_MAX - 1];
	struct timeval timestamps[RecoData::RT_MAX - 1];
	double belief[RecoData::RT_MAX - 1];
	pthread_spinlock_t locks[RecoData::RT_MAX - 1];
};

struct SharedMarkers
{
	MarkerNavi_t s_navi;
	MarkerIntersection_t s_intersection;
	MarkerTaillightImage_t s_taillightImage;
	MarkerObstacle_t s_obstacle;
	MarkerHokuyoObs_t s_hokuyoobs;
	MarkerCarFollowing_t s_carFollowing;
	MarkerVelocityDec_t s_velocityDec;
	MarkerLandMark_t s_landmark;
	MarkerLaneChange_t s_lanechange;
	MarkerParabola_t s_parabola;
	bool isValid[MarkerData::MARKER_MAX - 1];
	pthread_spinlock_t locks[MarkerData::MARKER_MAX - 1];
};

struct SharedMemory
{
	struct SharedConfig shm_config;
	struct SharedDecision shm_decision;
	struct SharedMetaData shm_metaData;
	struct SharedRecoData shm_recoData;
	struct SharedMarkers shm_markers;
};

class SharedObjectsImpl : public SharedObjects
{
protected:
	struct SharedMemory* m_addr;

	inline void Lock(pthread_spinlock_t* lock);
	inline void Unlock(pthread_spinlock_t* lock);

public:
	bool GetConfig(Config_t* config);
	void SetConfig(const Config_t& config);

	bool GetDecision(Decision_t* decision);
	bool SetDecision(const Decision_t& decision);

	bool SetMetaData(const MetaData_t& data, int index = 0);
	bool GetMetaData(MetaData* data, int index = 0, bool isGettingNewData = false);

	bool SetRecoData(const RecoData_t& data);
	bool GetRecoData(RecoData_t* data, bool isGettingNewData);

	bool SetMarker(const MarkerData_t& data);
	bool GetMarker(MarkerData_t* data);
};

}
}
