#pragma once

#include "types.h"

namespace module
{
namespace shm
{
/*****************************************************
 * Shared Object Structures
 */

// useful objects
#define SHARED_OBJECTS SharedObjects::GetInstance()

// smart marker number
#define SHM_SMART_MARKER_NUM 10

//POSIX unnamed semaphore
#define SEM_META_NUM 2 //add by Zou Lu
/*
 * following enum is used to index certain semaphore,
 * don't use numbers to index into the sems array, use this enum instead.
 * see actual implimentation in "Impl/ *"
 */
typedef enum {
      NAVI_FOR_HDL = 0 //used to indicate hdl capturer that a new navi data is ready
    , HDL_FOR_RECORDER = 1 //used to indicate hdl recorder that a new hdl data is ready
} SEM_META_ID; //add by Zou Lu

// SharedObject Class Definition
class SharedObjects
{
	/*
	 * Note: all the GET methods require a param pointer that would be used
	 *   to store the getting result. Thus users should allocate the memory
	 *   before calling these GET methods.
	 */
public:
	static SharedObjects& GetInstance();

	/*
	 * SET/GET shared decision object
	 */
	virtual bool SetDecision(const Decision_t& decision) = 0;
	virtual bool GetDecision(Decision_t* decision) = 0;

	/*
	 * SET/GET the meta data of the INDEX device
	 *
	 * Note: Callers should set the MetaData type before setting or getting
	 *   a MetaData. The meta data required by callers would be stored in the
	 *   data->value according to the data->type.
	 *
	 *   Setting a MetaData should be called in a Device Capture Process.
	 *
	 * Index: 0~2 BW Camera
	 *        0,1 Ibeo
	 *        0,1 Hokuyo
	 */
	virtual bool SetMetaData(const MetaData_t& data, int index = 0) = 0;
	virtual bool GetMetaData(MetaData* data, int index = 0, bool isGettingNewData = false) = 0;

	/*
	 * SET/GET the recognition data.
	 * Note: Callers should set the RecoData type before setting or getting
	 *   a RecoData. The reco data required by callers would be stored in the
	 *   data->value according to the data->type.
	 *
	 *   Setting an RecoData should be called in a Objects/Environment
	 *   Detecting/Recognition Process.
	 *
	 * params: isGettingNewData: if true, return new data from the last process.
	 *     Otherwise return the value;
	 */
	virtual bool SetRecoData(const RecoData_t& data) = 0;
	virtual bool GetRecoData(RecoData_t* data, bool isGettingNewData = false) = 0;

	/*
	 * SET/GET markers
	 * Note: Callers should set the MarkerData type before setting or getting
	 *   a MarkerData. The marker data required by callers would be stored in
	 *   the data->value according to the data->type.
	 */
	virtual bool SetMarker(const MarkerData_t& data) = 0;
	virtual bool GetMarker(MarkerData_t* data) = 0;

	/*
	 * SET/GET smart markers
	 * Note: Callers should set the marker index from 1 to SHM_SMART_MARKER_NUM
	 */
	virtual bool SetSmartMarker(int index) = 0;
	virtual bool GetSmartMarker(int index, bool isNew = false) = 0;
};

}
}
