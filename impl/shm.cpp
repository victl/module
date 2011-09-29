#include "shm.h"
#include "../log.h"
#include <signal.h>

using namespace module::logger;

namespace module
{

namespace shm
{
// Config
bool SharedObjectsImpl::GetConfig(Config_t* config)
{
	if(m_addr == NULL || config == NULL)
		return false;

	bool result = true;
	Lock(&m_addr->shm_config.lock);
	if(m_addr->shm_config.isValid)
		memcpy(config, &m_addr->shm_config.s_config, sizeof(Config_t));
	else
		result = false;
	Unlock(&m_addr->shm_config.lock);
	return result;
}

void SharedObjectsImpl::SetConfig(const Config_t& config)
{
	if(m_addr == NULL)
		return;

	Lock(&m_addr->shm_config.lock);
	memcpy(&m_addr->shm_config.s_config, &config, sizeof(Config_t));
	m_addr->shm_config.isValid = true;
	Unlock(&m_addr->shm_config.lock);
}

// Decision
bool SharedObjectsImpl::GetDecision(Decision_t* decision)
{
	if(m_addr == NULL || decision == NULL)
		return false;

	bool result = true;
	Lock(&m_addr->shm_decision.lock);
	if(m_addr->shm_decision.isValid)
		memcpy(decision, &m_addr->shm_decision.s_decision, sizeof(Decision_t));
	else
		result = false;
	Unlock(&m_addr->shm_decision.lock);
	return result;
}

bool SharedObjectsImpl::SetDecision(const Decision_t& decision)
{
	if(m_addr == NULL)
		return false;

	Lock(&m_addr->shm_decision.lock);
	memcpy(&m_addr->shm_decision.s_decision, &decision, sizeof(Decision_t));
	m_addr->shm_decision.isValid = true;
	Unlock(&m_addr->shm_decision.lock);
	return true;
}

// Meta Data
bool SharedObjectsImpl::SetMetaData(const MetaData_t& data, int index)
{
	if(m_addr == NULL)
		return false;

	switch(data.type)
	{
	case MetaData::META_NAVIGATION:
		Lock(&m_addr->shm_metaData.locks[0]);
		memcpy(&m_addr->shm_metaData.s_navi, &data.value.v_navi, sizeof(MetaNavigation_t));
		m_addr->shm_metaData.isValid[0] = true;
		m_addr->shm_metaData.isNew[0] = true;
		m_addr->shm_metaData.timestamps[0] = data.timestamp;
		Unlock(&m_addr->shm_metaData.locks[0]);
		return true;
	case MetaData::META_LASER_HDL:
		Lock(&m_addr->shm_metaData.locks[1]);
		memcpy(&m_addr->shm_metaData.s_laserHdl, &data.value.v_laserHdl, sizeof(MetaLaserHdl_t));
		m_addr->shm_metaData.isValid[1] = true;
		m_addr->shm_metaData.isNew[1] = true;
		m_addr->shm_metaData.timestamps[1] = data.timestamp;
		Unlock(&m_addr->shm_metaData.locks[1]);
		return true;
	case MetaData::META_CAMERA_BW:
		if(index >= 0 && index <= 2)
		{
			Lock(&m_addr->shm_metaData.locks[2 + index]);
			memcpy(&m_addr->shm_metaData.s_cameraBW[index], &data.value.v_cameraBW, sizeof(MetaCameraBW_t));
			m_addr->shm_metaData.isValid[2 + index] = true;
			m_addr->shm_metaData.isNew[2 + index] = true;
			m_addr->shm_metaData.timestamps[2 + index] = data.timestamp;
			Unlock(&m_addr->shm_metaData.locks[2 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_CAMERA_C:
		Lock(&m_addr->shm_metaData.locks[5]);
		memcpy(&m_addr->shm_metaData.s_cameraC, &data.value.v_cameraC, sizeof(MetaCameraC_t));
		m_addr->shm_metaData.isValid[5] = true;
		m_addr->shm_metaData.isNew[5] = true;
		m_addr->shm_metaData.timestamps[5] = data.timestamp;
		Unlock(&m_addr->shm_metaData.locks[5]);
		return true;
	case MetaData::META_ASL_POINTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[6 + index]);
			memcpy(&m_addr->shm_metaData.s_aslPts[index], &data.value.v_aslPts, sizeof(MetaAslPoints_t));
			m_addr->shm_metaData.isValid[6 + index] = true;
			m_addr->shm_metaData.isNew[6 + index] = true;
			m_addr->shm_metaData.timestamps[6 + index] = data.timestamp;
			Unlock(&m_addr->shm_metaData.locks[6 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_ASL_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[8 + index]);
			memcpy(&m_addr->shm_metaData.s_aslObjs[index], &data.value.v_aslObjs, sizeof(MetaAslObjects_t));
			m_addr->shm_metaData.isValid[8 + index] = true;
			m_addr->shm_metaData.isNew[8 + index] = true;
			m_addr->shm_metaData.timestamps[8 + index] = data.timestamp;
			Unlock(&m_addr->shm_metaData.locks[8 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_HOKUYO_POINTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[10 + index]);
			memcpy(&m_addr->shm_metaData.s_hokuyoPts[index], &data.value.v_hokuyoPts, sizeof(MetaHokuyoPoints_t));
			m_addr->shm_metaData.isValid[10 + index] = true;
			m_addr->shm_metaData.isNew[10 + index] = true;
			m_addr->shm_metaData.timestamps[10 + index] = data.timestamp;
			Unlock(&m_addr->shm_metaData.locks[10 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_HOKUYO_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[12 + index]);
			memcpy(&m_addr->shm_metaData.s_hokuyoObjs[index], &data.value.v_hokuyoObjs, sizeof(MetaHokuyoObjects_t));
			m_addr->shm_metaData.isValid[12 + index] = true;
			m_addr->shm_metaData.isNew[12 + index] = true;
			m_addr->shm_metaData.timestamps[12 + index] = data.timestamp;
			Unlock(&m_addr->shm_metaData.locks[12 + index]);
			return true;
		}
		else
			return false;
	default:
		return false;
	};
}

bool SharedObjectsImpl::GetMetaData(MetaData_t* data, int index, bool isGettingNewData)
{
	if(m_addr == NULL || data == NULL)
		return false;

	bool result = false;

	switch(data->type)
	{
	case MetaData::META_NAVIGATION:
		Lock(&m_addr->shm_metaData.locks[0]);
		if(m_addr->shm_metaData.isValid[0]
		        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[0])))
		{
			memcpy(&data->value.v_navi, &m_addr->shm_metaData.s_navi, sizeof(MetaNavigation_t));
			data->timestamp = m_addr->shm_metaData.timestamps[0];

			if(isGettingNewData && m_addr->shm_metaData.isNew[0])
				m_addr->shm_metaData.isNew[0] = false;

			result = true;
		}
		Unlock(&m_addr->shm_metaData.locks[0]);
		break;
	case MetaData::META_LASER_HDL:
		Lock(&m_addr->shm_metaData.locks[1]);
		if(m_addr->shm_metaData.isValid[1]
		        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[1])))
		{
			memcpy(&data->value.v_laserHdl, &m_addr->shm_metaData.s_laserHdl, sizeof(MetaLaserHdl_t));
			data->timestamp = m_addr->shm_metaData.timestamps[1];

			if(isGettingNewData && m_addr->shm_metaData.isNew[1])
				m_addr->shm_metaData.isNew[1] = false;

			result = true;
		}
		Unlock(&m_addr->shm_metaData.locks[1]);
		break;
	case MetaData::META_CAMERA_BW:
		if(index >= 0 && index <= 2)
		{
			Lock(&m_addr->shm_metaData.locks[2 + index]);
			if(m_addr->shm_metaData.isValid[2 + index]
			        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[2 + index])))
			{
				memcpy(&data->value.v_cameraBW, &m_addr->shm_metaData.s_cameraBW[index], sizeof(MetaCameraBW_t));
				data->timestamp = m_addr->shm_metaData.timestamps[2 + index];

				if(isGettingNewData && m_addr->shm_metaData.isNew[2 + index])
					m_addr->shm_metaData.isNew[2 + index] = false;

				result = true;
			}
			Unlock(&m_addr->shm_metaData.locks[2 + index]);
		}
		break;
	case MetaData::META_CAMERA_C:
		Lock(&m_addr->shm_metaData.locks[5]);
		if(m_addr->shm_metaData.isValid[5]
		        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[5])))
		{
			memcpy(&data->value.v_cameraC, &m_addr->shm_metaData.s_cameraC, sizeof(MetaCameraC_t));
			data->timestamp = m_addr->shm_metaData.timestamps[5];

			if(isGettingNewData && m_addr->shm_metaData.isNew[5])
				m_addr->shm_metaData.isNew[5] = false;

			result = true;
		}
		Unlock(&m_addr->shm_metaData.locks[5]);
		break;
	case MetaData::META_ASL_POINTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[6 + index]);
			if(m_addr->shm_metaData.isValid[6 + index]
			        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[6 + index])))
			{
				memcpy(&data->value.v_aslPts, &m_addr->shm_metaData.s_aslPts[index], sizeof(MetaAslPoints_t));
				data->timestamp = m_addr->shm_metaData.timestamps[6 + index];

				if(isGettingNewData && m_addr->shm_metaData.isNew[6 + index])
					m_addr->shm_metaData.isNew[6 + index] = false;

				result = true;
			}
			Unlock(&m_addr->shm_metaData.locks[6 + index]);
		}
		break;
	case MetaData::META_ASL_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[8 + index]);
			if(m_addr->shm_metaData.isValid[8 + index]
			        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[8 + index])))
			{
				memcpy(&data->value.v_aslObjs, &m_addr->shm_metaData.s_aslObjs[index], sizeof(MetaAslObjects_t));
				data->timestamp = m_addr->shm_metaData.timestamps[8 + index];
				if(isGettingNewData && m_addr->shm_metaData.isNew[8 + index])
					m_addr->shm_metaData.isNew[8 + index] = false;

				result = true;
			}
			Unlock(&m_addr->shm_metaData.locks[8 + index]);
		}
		break;
	case MetaData::META_HOKUYO_POINTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[10 + index]);
			if(m_addr->shm_metaData.isValid[10 + index]
			        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[10 + index])))
			{
				memcpy(&data->value.v_hokuyoPts, &m_addr->shm_metaData.s_hokuyoPts[index], sizeof(MetaHokuyoPoints_t));
				data->timestamp = m_addr->shm_metaData.timestamps[10 + index];

				if(isGettingNewData && m_addr->shm_metaData.isNew[10 + index])
					m_addr->shm_metaData.isNew[10 + index] = false;

				result = true;
			}
			Unlock(&m_addr->shm_metaData.locks[10 + index]);
		}
		break;
	case MetaData::META_HOKUYO_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			Lock(&m_addr->shm_metaData.locks[12 + index]);
			if(m_addr->shm_metaData.isValid[12 + index]
			        && (!isGettingNewData || (isGettingNewData && m_addr->shm_metaData.isNew[12 + index])))
			{
				memcpy(&data->value.v_hokuyoObjs, &m_addr->shm_metaData.s_hokuyoObjs[index], sizeof(MetaHokuyoObjects_t));
				data->timestamp = m_addr->shm_metaData.timestamps[12 + index];

				if(isGettingNewData && m_addr->shm_metaData.isNew[12 + index])
					m_addr->shm_metaData.isNew[12 + index] = false;

				result = true;
			}
			Unlock(&m_addr->shm_metaData.locks[12 + index]);
		}
		break;
	default:
		break;
	};
	return result;
}

// RecoData
bool SharedObjectsImpl::SetRecoData(const RecoData_t& data)
{
	if(m_addr == NULL || data.type <= 0 || data.type >= RecoData::RT_MAX)
		return false;

	int index = data.type - 1;
	Lock(&m_addr->shm_recoData.locks[index]);

	switch(data.type)
	{
	case RecoData::RT_OBSTACLE:
		memcpy(&m_addr->shm_recoData.s_obstacle, &data.value.v_obstacle, sizeof(RecoObstacle_t));
		break;
	case RecoData::RT_STOPLINE:
		memcpy(&m_addr->shm_recoData.s_stopline, &data.value.v_stopline, sizeof(RecoStopLine_t));
		break;
	case RecoData::RT_TRAFFICSIGN:
		memcpy(&m_addr->shm_recoData.s_ts, &data.value.v_ts, sizeof(RecoTrafficSign_t));
		break;
	case RecoData::RT_TRAFFICLIGHT:
		memcpy(&m_addr->shm_recoData.s_tl, &data.value.v_tl, sizeof(RecoTrafficLight_t));
		break;
	case RecoData::RT_INTERSECTION:
		memcpy(&m_addr->shm_recoData.s_intersection, &data.value.v_intersection, sizeof(RecoIntersection_t));
		break;
	case RecoData::RT_TRACK_LDAD:
		memcpy(&m_addr->shm_recoData.s_trackLdAd, &data.value.v_trackLdAd, sizeof(RecoTrackLdAd_t));
		break;
	case RecoData::RT_TRACK_PTS:
		memcpy(&m_addr->shm_recoData.s_trackPts, &data.value.v_trackPts, sizeof(RecoTrackPts_t));
		break;
	case RecoData::RT_SPOT:
		memcpy(&m_addr->shm_recoData.s_spot, &data.value.v_spot, sizeof(RecoSpot_t));
		break;
	case RecoData::RT_TRACK_ROAD:
		memcpy(&m_addr->shm_recoData.s_trackRoad, &data.value.v_trackRoad, sizeof(RecoTrackRoad_t));
		break;
	case RecoData::RT_SPOTLINE:
		memcpy(&m_addr->shm_recoData.s_spotline, &data.value.v_spotline, sizeof(RecoSpotLine_t));
		break;
	case RecoData::RT_SIDEPARKING:
		memcpy(&m_addr->shm_recoData.s_sideParking, &data.value.v_sideParking, sizeof(RecoSideParking_t));
		break;
	default:
		break;
	};
	m_addr->shm_recoData.isValid[index] = true;
	m_addr->shm_recoData.isNew[index] = true;
	if(data.timestamp.tv_sec == 0 && data.timestamp.tv_usec == 0)
		gettimeofday(&m_addr->shm_recoData.timestamps[index], NULL);
	else
		m_addr->shm_recoData.timestamps[index] = data.timestamp;
	m_addr->shm_recoData.belief[index] = data.belief;
	Unlock(&m_addr->shm_recoData.locks[index]);
	return true;
}

bool SharedObjectsImpl::GetRecoData(RecoData_t* data, bool isGettingNewData)
{
	if(m_addr == NULL || data == NULL || data->type <= 0 || data->type >= RecoData::RT_MAX)
		return false;

	int index = data->type - 1;
	bool result = false;

	Lock(&m_addr->shm_recoData.locks[index]);
	if(m_addr->shm_recoData.isValid[index]
	        && (!isGettingNewData || (isGettingNewData && m_addr->shm_recoData.isNew[index])))
	{
		switch(data->type)
		{
		case RecoData::RT_OBSTACLE:
			memcpy(&data->value.v_obstacle, &m_addr->shm_recoData.s_obstacle, sizeof(RecoObstacle_t));
			break;
		case RecoData::RT_STOPLINE:
			memcpy(&data->value.v_stopline, &m_addr->shm_recoData.s_stopline, sizeof(RecoStopLine_t));
			break;
		case RecoData::RT_TRAFFICSIGN:
			memcpy(&data->value.v_ts, &m_addr->shm_recoData.s_ts, sizeof(RecoTrafficSign_t));
			break;
		case RecoData::RT_TRAFFICLIGHT:
			memcpy(&data->value.v_tl, &m_addr->shm_recoData.s_tl, sizeof(RecoTrafficLight_t));
			break;
		case RecoData::RT_INTERSECTION:
			memcpy(&data->value.v_intersection, &m_addr->shm_recoData.s_intersection, sizeof(RecoIntersection_t));
			break;
		case RecoData::RT_TRACK_LDAD:
			memcpy(&data->value.v_trackLdAd, &m_addr->shm_recoData.s_trackLdAd, sizeof(RecoTrackLdAd_t));
			break;
		case RecoData::RT_TRACK_PTS:
			memcpy(&data->value.v_trackPts, &m_addr->shm_recoData.s_trackPts, sizeof(RecoTrackPts_t));
			break;
		case RecoData::RT_SPOT:
			memcpy(&data->value.v_spot, &m_addr->shm_recoData.s_spot, sizeof(RecoSpot_t));
			break;
		case RecoData::RT_TRACK_ROAD:
			memcpy(&data->value.v_trackRoad, &m_addr->shm_recoData.s_trackRoad, sizeof(RecoTrackRoad_t));
			break;
		case RecoData::RT_SPOTLINE:
			memcpy(&data->value.v_spotline, &m_addr->shm_recoData.s_spotline, sizeof(RecoSpotLine_t));
			break;
		case RecoData::RT_SIDEPARKING:
			memcpy(&data->value.v_sideParking, &m_addr->shm_recoData.s_sideParking, sizeof(RecoSideParking_t));
			break;
		};

		data->timestamp = m_addr->shm_recoData.timestamps[index];
		data->belief = m_addr->shm_recoData.belief[index];
		if(isGettingNewData && m_addr->shm_recoData.isNew[index])
			m_addr->shm_recoData.isNew[index] = false;

		result = true;
	}
	Unlock(&m_addr->shm_recoData.locks[index]);
	return result;
}

// Marker
bool SharedObjectsImpl::SetMarker(const MarkerData_t& data)
{
	if(m_addr == NULL || data.type <= 0 || data.type >= MarkerData::MARKER_MAX)
		return false;

	int index = data.type - 1;
	Lock(&m_addr->shm_markers.locks[index]);
	switch(data.type)
	{
	case MarkerData::MARKER_NAVIGATION:
		memcpy(&m_addr->shm_markers.s_navi, &data.value.v_navi, sizeof(MarkerNavi_t));
		break;
	case MarkerData::MARKER_INTERSECTION:
		memcpy(&m_addr->shm_markers.s_intersection, &data.value.v_intersection, sizeof(MarkerIntersection_t));
		break;
	case MarkerData::MARKER_TAILLIGHT_IMAGE:
		memcpy(&m_addr->shm_markers.s_taillightImage, &data.value.v_taillightImage, sizeof(MarkerTaillightImage_t));
		break;
	case MarkerData::MARKER_OBSTACLE:
		memcpy(&m_addr->shm_markers.s_obstacle, &data.value.v_obstacle, sizeof(MarkerObstacle_t));
		break;
	case MarkerData::MARKER_HOKUYO_OBS:
		memcpy(&m_addr->shm_markers.s_hokuyoobs, &data.value.v_hokuyoobs, sizeof(MarkerHokuyoObs_t));
		break;
	case MarkerData::MARKER_CAR_FOLLOWING:
		memcpy(&m_addr->shm_markers.s_carFollowing, &data.value.v_carFollowing, sizeof(MarkerCarFollowing_t));
		break;
	case MarkerData::MARKER_VELOCITY_DEC:
		memcpy(&m_addr->shm_markers.s_velocityDec, &data.value.v_velocityDec, sizeof(MarkerVelocityDec_t));
		break;
	case MarkerData::MARKER_LANDMARK:
		memcpy(&m_addr->shm_markers.s_landmark, &data.value.v_landmark, sizeof(MarkerLandMark_t));
		break;
	case MarkerData::MARKER_LANECHANGE:
		memcpy(&m_addr->shm_markers.s_lanechange, &data.value.v_lanechange, sizeof(MarkerLaneChange_t));
		break;
	case MarkerData::MARKER_PARABOLA:
		memcpy(&m_addr->shm_markers.s_parabola, &data.value.v_parabola, sizeof(MarkerParabola_t));
		break;
	default:
		break;
	};

	m_addr->shm_markers.isValid[index] = true;
	Unlock(&m_addr->shm_markers.locks[index]);
	return true;
}

bool SharedObjectsImpl::GetMarker(MarkerData_t* data)
{
	if(m_addr == NULL || data == NULL || data->type <= 0 || data->type >= MarkerData::MARKER_MAX)
		return false;

	int index = data->type - 1;
	bool result = true;

	Lock(&m_addr->shm_markers.locks[index]);
	if(m_addr->shm_markers.isValid[index])
	{
		switch(data->type)
		{
		case MarkerData::MARKER_NAVIGATION:
			memcpy(&data->value.v_navi, &m_addr->shm_markers.s_navi, sizeof(MarkerNavi_t));
			break;
		case MarkerData::MARKER_INTERSECTION:
			memcpy(&data->value.v_intersection, &m_addr->shm_markers.s_intersection, sizeof(MarkerIntersection_t));
			break;
		case MarkerData::MARKER_TAILLIGHT_IMAGE:
			memcpy(&data->value.v_taillightImage, &m_addr->shm_markers.s_taillightImage, sizeof(MarkerTaillightImage_t));
			break;
		case MarkerData::MARKER_OBSTACLE:
			memcpy(&data->value.v_obstacle, &m_addr->shm_markers.s_obstacle, sizeof(MarkerObstacle_t));
			break;
		case MarkerData::MARKER_HOKUYO_OBS:
			memcpy(&data->value.v_hokuyoobs, &m_addr->shm_markers.s_hokuyoobs, sizeof(MarkerHokuyoObs_t));
			break;
		case MarkerData::MARKER_CAR_FOLLOWING:
			memcpy(&data->value.v_carFollowing, &m_addr->shm_markers.s_carFollowing, sizeof(MarkerCarFollowing_t));
			break;
		case MarkerData::MARKER_VELOCITY_DEC:
			memcpy(&data->value.v_velocityDec, &m_addr->shm_markers.s_velocityDec, sizeof(MarkerVelocityDec_t));
			break;
		case MarkerData::MARKER_LANDMARK:
			memcpy(&data->value.v_landmark, &m_addr->shm_markers.s_landmark, sizeof(MarkerLandMark_t));
			break;
		case MarkerData::MARKER_LANECHANGE:
			memcpy(&data->value.v_lanechange, &m_addr->shm_markers.s_lanechange, sizeof(MarkerLaneChange_t));
			break;
		case MarkerData::MARKER_PARABOLA:
			memcpy(&data->value.v_parabola, &m_addr->shm_markers.s_parabola, sizeof(MarkerParabola_t));
			break;
		default:
			result = false;
			break;
		};
	}
	else
		result = false;

	Unlock(&m_addr->shm_markers.locks[index]);
	return result;
}

void SharedObjectsImpl::Lock(pthread_spinlock_t* lock)
{
	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGINT);
	if(sigprocmask(SIG_BLOCK, &newset, NULL) < 0)
	{
		LOG_ERROR_WITH_NO("sigprocmask");
	}

	pthread_spin_lock(lock);
}

void SharedObjectsImpl::Unlock(pthread_spinlock_t* lock)
{
	pthread_spin_unlock(lock);

	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGINT);
	if(sigprocmask(SIG_UNBLOCK, &newset, NULL) < 0)
	{
		LOG_ERROR_WITH_NO("sigprocmask");
	}
}
}
}
