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
	// device capture meta data
	MODULE_CAPTURE_HDL               = 2,
	MODULE_CAPTURE_PYLON             = 3,
	MODULE_CAPTURE_PYLON_MONO        = 4,
	MODULE_CAPTURE_ASL               = 5,
	MODULE_CAPTURE_HOKUYO            = 6,

	// navigation
	MODULE_NAV_GPS                   = 7,
	MODULE_NAV_MAP                   = 8,

	// basic functions (without decision)
	MODULE_TRAFFICSIGN_RECOGNITION   = 9,
	MODULE_TRAFFICLIGHT_RECOGNITION  = 10,
	MODULE_OBSTACLE_DETECTING_IBEO   = 11,
	MODULE_OBSTACLE_DETECTING_HOKUYO = 12,
	MODULE_LANE_TRACKING             = 13,
	MODULE_CURB_TRACKING             = 14,
	MODULE_LANE_CURB_TRACKING        = 15,
	MODULE_VEHICLE_FOLLOWING         = 16,
	MODULE_S_CURVE                   = 17,
	MODULE_PARKING_1                 = 18,
	MODULE_PARKING_2                 = 19,
	MODULE_CHANGE_PATH				 = 20
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
#define MODULE_CAPTURE_HDL_NAME               "./ModuleCapHdl"
#define MODULE_CAPTURE_PYLON_NAME             "./ModuleCapPylon"
#define MODULE_CAPTURE_PYLON_MONO_NAME        "./ModuleCapPylonMono"
#define MODULE_CAPTURE_ASL_NAME               "./ModuleCapAsl"
#define MODULE_CAPTURE_HOKUYO_NAME            "./ModuleCapHokuyo"

#define MODULE_NAV_GPS_NAME                   "./ModuleNavGps"
#define MODULE_NAV_MAP_NAME                   "./ModuleNavMap"

#define MODULE_TRAFFICSIGN_RECOGNITION_NAME   "./ModuleTS"
#define MODULE_TRAFFICLIGHT_RECOGNITION_NAME  "./ModuleTL"
#define MODULE_OBSTACLE_DETECTING_IBEO_NAME   "./ModuleODIbeo"
#define MODULE_OBSTACLE_DETECTING_IBEO_HOKUYO "./ModuleODHokuyo"
#define MODULE_LANE_TRACKING_NAME             "./ModuleLT"
#define MODULE_CURB_TRACKING_NAME             "./ModuleCT"
#define MODULE_LANE_CURB_TRACKING_NAME        "./ModuleLCT"
#define MODULE_VEHICLE_FOLLOWING_NAME         "./ModuleVF"
#define MODULE_S_CURVE_NAME                   "./ModuleSC"
#define MODULE_PARKING_1_NAME                 "./ModuleP1"
#define MODULE_PARKING_2_NAME                 "./ModuleP2"
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
