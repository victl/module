#include "shm.h"
#include "../log.h"

using namespace module::log;

namespace module
{

namespace shm
{
// Decision
bool SharedObjectsImpl::GetDecision(Decision_t* decision) const
{
	if(m_addr == NULL || decision == NULL)
		return false;

	bool result = true;
	pthread_spin_lock(&m_addr->shm_decision.lock);
	if(m_addr->shm_decision.isValid)
		memcpy(decision, &m_addr->shm_decision.s_decision, sizeof(Decision_t));
	else
		result = false;
	pthread_spin_unlock(&m_addr->shm_decision.lock);
	return result;
}

bool SharedObjectsImpl::SetDecision(const Decision_t& decision)
{
	if(m_addr == NULL)
		return false;

	pthread_spin_lock(&m_addr->shm_decision.lock);
	memcpy(&m_addr->shm_decision.s_decision, &decision, sizeof(Decision_t));
	m_addr->shm_decision.isValid = true;
	pthread_spin_unlock(&m_addr->shm_decision.lock);
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
		pthread_spin_lock(&m_addr->shm_metaData.locks[0]);
		memcpy(&m_addr->shm_metaData.s_navi, &data.value.v_navi, sizeof(MetaNavigation_t));
		m_addr->shm_metaData.isValid[0] = true;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[0]);
		return true;
	case MetaData::META_LASER_HDL:
		pthread_spin_lock(&m_addr->shm_metaData.locks[1]);
		memcpy(&m_addr->shm_metaData.s_laserHdl, &data.value.v_laserHdl, sizeof(MetaLaserHdl_t));
		m_addr->shm_metaData.isValid[1] = true;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[1]);
		return true;
	case MetaData::META_CAMERA_BW:
		if(index >= 0 && index <= 2)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[2 + index]);
			memcpy(&m_addr->shm_metaData.s_cameraBW[index], &data.value.v_cameraBW, sizeof(MetaCameraBW_t));
			m_addr->shm_metaData.isValid[2 + index] = true;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[2 + index]);
		}
		else
			return false;
	case MetaData::META_CAMERA_C:
		pthread_spin_lock(&m_addr->shm_metaData.locks[5]);
		memcpy(&m_addr->shm_metaData.s_cameraC, &data.value.v_cameraC, sizeof(MetaCameraC_t));
		m_addr->shm_metaData.isValid[5] = true;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[5]);
		return true;
	case MetaData::META_ASL_POINTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[6 + index]);
			memcpy(&m_addr->shm_metaData.s_aslPts[index], &data.value.v_aslPts, sizeof(MetaAslPoints_t));
			m_addr->shm_metaData.isValid[6 + index] = true;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[6 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_ASL_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[8 + index]);
			memcpy(&m_addr->shm_metaData.s_aslObjs[index], &data.value.v_aslObjs, sizeof(MetaAslObjects_t));
			m_addr->shm_metaData.isValid[8 + index] = true;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[8 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_HOKUYO_POINTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[10 + index]);
			memcpy(&m_addr->shm_metaData.s_hokuyoPts[index], &data.value.v_hokuyoPts, sizeof(MetaHokuyoPoints_t));
			m_addr->shm_metaData.isValid[10 + index] = true;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[10 + index]);
			return true;
		}
		else
			return false;
	case MetaData::META_HOKUYO_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[12 + index]);
			memcpy(&m_addr->shm_metaData.s_hokuyoObjs[index], &data.value.v_hokuyoObjs, sizeof(MetaHokuyoObjects_t));
			m_addr->shm_metaData.isValid[12 + index] = true;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[12 + index]);
			return true;
		}
		else
			return false;
	default:
		return false;
	};
}

bool SharedObjectsImpl::GetMetaData(MetaData_t* data, int index) const
{
	if(m_addr == NULL || data == NULL)
		return false;

	bool result = true;

	switch(data->type)
	{
	case MetaData::META_NAVIGATION:
		pthread_spin_lock(&m_addr->shm_metaData.locks[0]);
		if(m_addr->shm_metaData.isValid[0])
			memcpy(&data->value.v_navi, &m_addr->shm_metaData.s_navi, sizeof(MetaNavigation_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[0]);
		break;
	case MetaData::META_LASER_HDL:
		pthread_spin_lock(&m_addr->shm_metaData.locks[1]);
		if(m_addr->shm_metaData.isValid[1])
			memcpy(&data->value.v_laserHdl, &m_addr->shm_metaData.s_laserHdl, sizeof(MetaLaserHdl_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[1]);
		break;
	case MetaData::META_CAMERA_BW:
		if(index >= 0 && index <= 2)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[2 + index]);
			if(m_addr->shm_metaData.isValid[2 + index])
				memcpy(&data->value.v_cameraBW, &m_addr->shm_metaData.s_cameraBW[index], sizeof(MetaCameraBW_t));
			else
				result = false;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[2 + index]);
		}
		else
			result = false;
		break;
	case MetaData::META_CAMERA_C:
		pthread_spin_lock(&m_addr->shm_metaData.locks[5]);
		if(m_addr->shm_metaData.isValid[5])
			memcpy(&data->value.v_cameraC, &m_addr->shm_metaData.s_cameraC, sizeof(MetaCameraC_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_metaData.locks[5]);
		break;
	case MetaData::META_ASL_POINTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[6 + index]);
			if(m_addr->shm_metaData.isValid[6 + index])
				memcpy(&data->value.v_aslPts, &m_addr->shm_metaData.s_aslPts[index], sizeof(MetaAslPoints_t));
			else
				result = false;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[6 + index]);
		}
		else
			result = false;
		break;
	case MetaData::META_ASL_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[8 + index]);
			if(m_addr->shm_metaData.isValid[8 + index])
				memcpy(&data->value.v_aslObjs, &m_addr->shm_metaData.s_aslObjs[index], sizeof(MetaAslObjects_t));
			else
				result = false;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[8 + index]);
		}
		else
			result = false;
		break;
	case MetaData::META_HOKUYO_POINTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[10 + index]);
			if(m_addr->shm_metaData.isValid[10 + index])
				memcpy(&data->value.v_hokuyoPts, &m_addr->shm_metaData.s_hokuyoPts[index], sizeof(MetaHokuyoPoints_t));
			else
				result = false;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[10 + index]);
		}
		else
			result = false;
		break;
	case MetaData::META_HOKUYO_OBJECTS:
		if(index >= 0 && index <= 1)
		{
			pthread_spin_lock(&m_addr->shm_metaData.locks[12 + index]);
			if(m_addr->shm_metaData.isValid[12 + index])
				memcpy(&data->value.v_hokuyoObjs, &m_addr->shm_metaData.s_hokuyoObjs[index], sizeof(MetaHokuyoObjects_t));
			else
				result = false;
			pthread_spin_unlock(&m_addr->shm_metaData.locks[12 + index]);
		}
		else
			result = false;
		break;
	default:
		result = false;
		break;
	};
	return result;
}

// RecoData
bool SharedObjectsImpl::SetRecoData(const RecoData_t& data)
{
	if(m_addr == NULL)
		return false;

	switch(data.type)
	{
	case RecoData::RT_OBSTACLE:
		pthread_spin_lock(&m_addr->shm_recoData.locks[0]);
		memcpy(&m_addr->shm_recoData.s_obstacle, &data.value.v_obstacle, sizeof(RecoObstacle_t));
		m_addr->shm_recoData.isValid[0] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[0]);
		return true;
	case RecoData::RT_STOPLINE:
		pthread_spin_lock(&m_addr->shm_recoData.locks[1]);
		memcpy(&m_addr->shm_recoData.s_stopline, &data.value.v_stopline, sizeof(RecoStopLine_t));
		m_addr->shm_recoData.isValid[1] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[1]);
		return true;
	case RecoData::RT_TRAFFICSIGN:
		pthread_spin_lock(&m_addr->shm_recoData.locks[2]);
		memcpy(&m_addr->shm_recoData.s_ts, &data.value.v_ts, sizeof(RecoTrafficSign_t));
		m_addr->shm_recoData.isValid[2] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[2]);
		return true;
	case RecoData::RT_TRAFFICLIGHT:
		pthread_spin_lock(&m_addr->shm_recoData.locks[3]);
		memcpy(&m_addr->shm_recoData.s_tl, &data.value.v_tl, sizeof(RecoTrafficLight_t));
		m_addr->shm_recoData.isValid[3] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[3]);
		return true;
	case RecoData::RT_INTERSECTION:
		pthread_spin_lock(&m_addr->shm_recoData.locks[4]);
		memcpy(&m_addr->shm_recoData.s_intersection, &data.value.v_intersection, sizeof(RecoIntersection_t));
		m_addr->shm_recoData.isValid[4] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[4]);
		return true;
	case RecoData::RT_TRACK_LDAD:
		pthread_spin_lock(&m_addr->shm_recoData.locks[5]);
		memcpy(&m_addr->shm_recoData.s_trackLdAd, &data.value.v_trackLdAd, sizeof(RecoTrackLdAd_t));
		m_addr->shm_recoData.isValid[5] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[5]);
		return true;
	case RecoData::RT_TRACK_PTS:
		pthread_spin_lock(&m_addr->shm_recoData.locks[6]);
		memcpy(&m_addr->shm_recoData.s_trackPts, &data.value.v_trackPts, sizeof(RecoTrackPts_t));
		m_addr->shm_recoData.isValid[6] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[6]);
		return true;
	case RecoData::RT_SPOT:
		pthread_spin_lock(&m_addr->shm_recoData.locks[7]);
		memcpy(&m_addr->shm_recoData.s_spot, &data.value.v_spot, sizeof(RecoSpot_t));
		m_addr->shm_recoData.isValid[7] = true;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[7]);
		return true;
	default:
		return false;
	};
}

bool SharedObjectsImpl::GetRecoData(RecoData_t* data) const
{
	if(m_addr == NULL)
		return false;

	bool result = true;
	switch(data->type)
	{
	case RecoData::RT_OBSTACLE:
		pthread_spin_lock(&m_addr->shm_recoData.locks[0]);
		if(m_addr->shm_recoData.isValid[0])
			memcpy(&data->value.v_obstacle, &m_addr->shm_recoData.s_obstacle, sizeof(RecoObstacle_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[0]);
		break;
	case RecoData::RT_STOPLINE:
		pthread_spin_lock(&m_addr->shm_recoData.locks[1]);
		if(m_addr->shm_recoData.isValid[1])
			memcpy(&data->value.v_stopline, &m_addr->shm_recoData.s_stopline, sizeof(RecoStopLine_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[1]);
		break;
	case RecoData::RT_TRAFFICSIGN:
		pthread_spin_lock(&m_addr->shm_recoData.locks[2]);
		if(m_addr->shm_recoData.isValid[2])
			memcpy(&data->value.v_ts, &m_addr->shm_recoData.s_ts, sizeof(RecoTrafficSign_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[2]);
		break;
	case RecoData::RT_TRAFFICLIGHT:
		pthread_spin_lock(&m_addr->shm_recoData.locks[3]);
		if(m_addr->shm_recoData.isValid[3])
			memcpy(&data->value.v_tl, &m_addr->shm_recoData.s_tl, sizeof(RecoTrafficLight_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[3]);
		break;
	case RecoData::RT_INTERSECTION:
		pthread_spin_lock(&m_addr->shm_recoData.locks[4]);
		if(m_addr->shm_recoData.isValid[4])
			memcpy(&data->value.v_intersection, &m_addr->shm_recoData.s_intersection, sizeof(RecoIntersection_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[4]);
		break;
	case RecoData::RT_TRACK_LDAD:
		pthread_spin_lock(&m_addr->shm_recoData.locks[5]);
		if(m_addr->shm_recoData.isValid[5])
			memcpy(&data->value.v_trackLdAd, &m_addr->shm_recoData.s_trackLdAd, sizeof(RecoTrackLdAd_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[5]);
		break;
	case RecoData::RT_TRACK_PTS:
		pthread_spin_lock(&m_addr->shm_recoData.locks[6]);
		if(m_addr->shm_recoData.isValid[6])
			memcpy(&data->value.v_trackPts, &m_addr->shm_recoData.s_trackPts, sizeof(RecoTrackPts_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[6]);
		break;
	case RecoData::RT_SPOT:
		pthread_spin_lock(&m_addr->shm_recoData.locks[7]);
		if(m_addr->shm_recoData.isValid[7])
			memcpy(&data->value.v_spot, &m_addr->shm_recoData.s_spot, sizeof(RecoSpot_t));
		else
			result = false;
		pthread_spin_unlock(&m_addr->shm_recoData.locks[7]);
		break;
	default:
		result = false;
	};
	return result;
}

// Marker
bool SharedObjectsImpl::SetMarker(const MarkerData_t& data)
{
	if(m_addr == NULL)
		return false;

	switch(data.type)
	{
	case MarkerData::MARKER_NAVIGATION:
		pthread_spin_lock(&m_addr->shm_markers.locks[0]);
		memcpy(&m_addr->shm_markers.s_navi, &data.value.v_navi, sizeof(MarkerNavi_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[0]);
		return true;
	case MarkerData::MARKER_INTERSECTION:
		pthread_spin_lock(&m_addr->shm_markers.locks[1]);
		memcpy(&m_addr->shm_markers.s_intersection, &data.value.v_intersection, sizeof(MarkerIntersection_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[1]);
		return true;
	case MarkerData::MARKER_TAILLIGHT_IMAGE:
		pthread_spin_lock(&m_addr->shm_markers.locks[2]);
		memcpy(&m_addr->shm_markers.s_taillightImage, &data.value.v_taillightImage, sizeof(MarkerTaillightImage_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[2]);
		return true;
	case MarkerData::MARKER_OBSTACLE:
		pthread_spin_lock(&m_addr->shm_markers.locks[3]);
		memcpy(&m_addr->shm_markers.s_obstacle, &data.value.v_obstacle, sizeof(MarkerObstacle_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[3]);
		return true;
	case MarkerData::MARKER_HOKUYO_OBS:
		pthread_spin_lock(&m_addr->shm_markers.locks[4]);
		memcpy(&m_addr->shm_markers.s_hokuyoobs, &data.value.v_hokuyoobs, sizeof(MarkerHokuyoObs_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[4]);
		return true;
	default:
		return false;
	};
}

bool SharedObjectsImpl::GetMarker(MarkerData_t* data) const
{
	if(m_addr == NULL || data == NULL)
		return false;

	switch(data->type)
	{
	case MarkerData::MARKER_NAVIGATION:
		pthread_spin_lock(&m_addr->shm_markers.locks[0]);
		memcpy(&data->value.v_navi, &m_addr->shm_markers.s_navi, sizeof(MarkerNavi_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[0]);
		return true;
	case MarkerData::MARKER_INTERSECTION:
		pthread_spin_lock(&m_addr->shm_markers.locks[1]);
		memcpy(&data->value.v_intersection, &m_addr->shm_markers.s_intersection, sizeof(MarkerIntersection_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[1]);
		return true;
	case MarkerData::MARKER_TAILLIGHT_IMAGE:
		pthread_spin_lock(&m_addr->shm_markers.locks[2]);
		memcpy(&data->value.v_taillightImage, &m_addr->shm_markers.s_taillightImage, sizeof(MarkerTaillightImage_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[2]);
		return true;
	case MarkerData::MARKER_OBSTACLE:
		pthread_spin_lock(&m_addr->shm_markers.locks[3]);
		memcpy(&data->value.v_obstacle, &m_addr->shm_markers.s_obstacle, sizeof(MarkerObstacle_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[3]);
		return true;
	case MarkerData::MARKER_HOKUYO_OBS:
		pthread_spin_lock(&m_addr->shm_markers.locks[4]);
		memcpy(&data->value.v_hokuyoobs, &m_addr->shm_markers.s_hokuyoobs, sizeof(MarkerHokuyoObs_t));
		pthread_spin_unlock(&m_addr->shm_markers.locks[4]);
		return true;
	default:
		return false;
	};
}
}
}
