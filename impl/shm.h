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
	MetaLocalNavigation_t s_navi_local;
	MetaLaserHdl_t s_laserHdl;
	MetaCameraBW_t s_cameraBW[3];
	MetaCameraC_t s_cameraC;
	MetaLUXPoints_t s_luxPts;
	MetaLUXObjects_t s_luxObjs;
//	MetaHokuyoPoints_t s_hokuyoPts[2];
//	MetaHokuyoObjects_t s_hokuyoObjs[2];
	bool isValid[9];
	bool isNew[9];
	struct timeval timestamps[9];
	pthread_spinlock_t locks[9];
};

struct SharedRecoData
{
	RecoStopLine_t s_stopline;
	RecoTrafficSign_t s_ts;
	RecoTrafficLight_t s_tl;
	//RecoTrackLdAd_t s_trackLdAd;
	RecoTrackLocalAndPreview_t s_trackLocalAndPreview;
	RecoSlowDown_t s_slowdown;
	RecoEmergency_t s_emergency;
	RecoSideObs_t s_sideObs;
	RecoLaneMark_t s_lanemark;
	RecoLaneMark_t s_curb;
	RecoLaneMark_t s_rail;
	RecoSCurve_t s_scurve;
	Pose_t s_guide_pt;
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
//	MarkerTaillightImage_t s_taillightImage;
//	MarkerHokuyoObs_t s_hokuyoobs;
//	MarkerVelocityDec_t s_velocityDec;
	MarkerVelocityDecLux_t s_velocityDecLux;
	MarkerLaneChange_t s_lanechange;
	MarkerParabola_t s_parabola;
	MarkerLaneChangeObstacle_t s_lanechangeobstacle;
//	MarkerLaneChangeSide_t s_lanechangeside;
//	MarkerObstacle_t s_obstacle;
	MarkerObstacleLux_t s_obstacleLux;
	MarkerTrafficLight_t s_tl;
//	MarkerInitSensor_t s_initsensor;
	MarkerRoadTracking_t s_roadtracking;
	bool isValid[MarkerData::MARKER_MAX - 1];
	pthread_spinlock_t locks[MarkerData::MARKER_MAX - 1];
};

struct SharedSmartMarkers
{
	bool isValid[SHM_SMART_MARKER_NUM];
	bool isNew[SHM_SMART_MARKER_NUM];
	pthread_spinlock_t locks[10];
};

struct SharedMemory
{
	struct SharedDecision shm_decision;
	struct SharedMetaData shm_metaData;
	struct SharedRecoData shm_recoData;
	struct SharedMarkers shm_markers;
	struct SharedSmartMarkers shm_smartMarkers;
};

class SharedObjectsImpl : public SharedObjects
{
protected:
	struct SharedMemory* m_addr;

	static inline void Lock(pthread_spinlock_t* lock);
	static inline void Unlock(pthread_spinlock_t* lock);

public:
	bool GetDecision(Decision_t* decision);
	bool SetDecision(const Decision_t& decision);

	bool SetMetaData(const MetaData_t& data, int index = 0);
	bool GetMetaData(MetaData* data, int index = 0, bool isGettingNewData = false);

	bool SetRecoData(const RecoData_t& data);
	bool GetRecoData(RecoData_t* data, bool isGettingNewData = false);

	bool SetMarker(const MarkerData_t& data);
	bool GetMarker(MarkerData_t* data);

	bool SetSmartMarker(int index);
	bool GetSmartMarker(int index, bool isNew = false);
};

}
}
