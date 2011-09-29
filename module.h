#pragma once

namespace module
{
/*
 * modules enum
 */
enum
{
	// controller
	MODULE_CONTROLLER                = 0,
	// decision
	MODULE_DECISION                  = 1,
	// emergency
	MODULE_EMERGENCY                 = 2,
	// slowdown
	MODULE_SLOWDOWN                  = 3,
	// device capture meta data
	MODULE_CAPTURE_HDL               = 4,
	MODULE_CAPTURE_PYLON             = 5,
	MODULE_CAPTURE_PYLON_MONO        = 6,
	MODULE_CAPTURE_HOKUYO            = 7,

	// navigation
	MODULE_NAV_GPS                   = 8,

	// basic functions (without decision)
	MODULE_TRAFFICSIGN_RECOGNITION   = 9,
	MODULE_TRAFFICLIGHT_RECOGNITION  = 10,
	MODULE_ROAD_TRACKING             = 11,
	MODULE_VEHICLE_FOLLOWING         = 12,
	MODULE_S_CURVE                   = 13,
	MODULE_PARKING_1                 = 14,
	MODULE_SIDE_PARKING              = 15,
	MODULE_CHANGE_PATH				 = 16
};

// utils macro
#define MODULE_INDEX_MIN MODULE_CONTROLLER
#define MODULE_INDEX_MAX MODULE_CHANGE_PATH
#define MODULE_NUM (MODULE_INDEX_MAX - MODULE_INDEX_MIN + 1)

/*
 * modules name
 * Note: users could use these names to generate the module execute file
 */
#define MODULE_CONTROLLER_NAME                "./ModuleController"
#define MODULE_DECISION_NAME                  "./ModuleDecision"
#define MODULE_EMERGENCY_NAME                 "./ModuleEmergency"
#define MODULE_SLOWDOWN_NAME                  "./ModuleSlowDown"

#define MODULE_CAPTURE_HDL_NAME               "./ModuleCapHdl"
#define MODULE_CAPTURE_PYLON_NAME             "./ModuleCapPylon"
#define MODULE_CAPTURE_PYLON_MONO_NAME        "./ModuleCapPylonMono"
//#define MODULE_CAPTURE_ASL_NAME               "./ModuleCapAsl"
#define MODULE_CAPTURE_HOKUYO_NAME            "./ModuleCapHokuyo"

#define MODULE_NAV_GPS_NAME                   "./ModuleNavGps"
//#define MODULE_NAV_MAP_NAME                   "./ModuleNavMap"

#define MODULE_TRAFFICSIGN_RECOGNITION_NAME   "./ModuleTS"
#define MODULE_TRAFFICLIGHT_RECOGNITION_NAME  "./ModuleTL"

#define MODULE_ROAD_TRACKING_NAME             "./ModuleRoadTracking"
#define MODULE_VEHICLE_FOLLOWING_NAME         "./ModuleVF"
#define MODULE_S_CURVE_NAME                   "./ModuleSC"
#define MODULE_PARKING_1_NAME                 "./ModuleP1"
#define MODULE_SIDE_PARKING_NAME              "./ModuleSideParking"
//#define MODULE_PARKING_2_NAME                 "./ModuleP2"
#define MODULE_CHANGE_PATH_NAME               "./ModuleCP"

/*
 * define a cleanup handler called when interrupted or abort
 * by the Main Control
 */
typedef void (*ModuleCleanup)(void);

/*
 * users should register a cleanup handler if some devices may
 * need to destroy specific resources or release system memories.
 *
 * Note: only the last one handler is registered if this method
 *   is called more than once.
 */
void RegisterCleanup(ModuleCleanup cleanup);
void UnRegisterCleanup();

}
