#ifndef __TYPES_H_
#define __TYPES_H_
#include <sys/time.h>
#include <stdio.h>
#include <semaphore.h>
#include <vector>
#include <math.h>
namespace module
{
/****************************************************
 * device meta data macro
 */
//Ibeo(LUX) meta data size
#define LUX_MAX_POINTS         11360
#define LUX_MAX_OBJECTS        256
#define LUX_MAX_CONTOURPOINTS  256
// HDL meta point number
#define HDL_LASER_NUMBER       64
#define HDL_MAX_POINT_LASER    4000
#define HDL_MAX_POINT_NUMBER HDL_MAX_POINT_LASER * HDL_LASER_NUMBER
// Black-White(BW) camera meta data size
#define BWCAM_IMAGE_WIDTH      780
#define BWCAM_IMAGE_HEIGHT     582
#define BWCAM_IMAGE_SIZE BWCAM_IMAGE_WIDTH * BWCAM_IMAGE_HEIGHT
// Color(C) camera meta data size
#define CCAM_IMAGE_WIDTH       1292
#define CCAM_IMAGE_HEIGHT      964
#define C_WIDTH                3
#define CCAM_IMAGE_SIZE CCAM_IMAGE_WIDTH * CCAM_IMAGE_HEIGHT * C_WIDTH
// decision guide pts num
#define MAX_GUIDE_PTS          10
// Ibeo(ASL) meta data size
//#define ASL_MAX_POINTS 8648
//#define ASL_MAX_CONTOURPOINTS	16
//#define ASL_MAX_OBJECTS		64
// Hokuyo meta data size
//#define HOKUYO_MAX_POINTS 1081
//#define HOKUYO_MAX_OBJECTS  20
//#define HOKUYO_MAX_CONTOURPOINTS 150
/****************************************************
 * Decision Structure (controller <--> decision)
 */
typedef struct GuidePts
{
	double x;
	double y;
	double z;
} GuidePts_t;

typedef struct Decision
{
	bool isOverride;
	int currGuidePtIndex;
	double LDevi;
	double AzimuthDevi;
	double DesiredVelocity;
	double VehicleVelocity;
	double Acceleration;
	char Shift;
	bool Leftlight;
	bool Rightlight;
	enum
	{
	    VS_START        = 1,
	    VS_STOP         = 2,
	    VS_PAUSE        = 3,
	    VS_EMERGENCY    = 4,
	    VS_RUNNING      = 5
	} State;
	enum BehaviorType
	{
	    BEHAVIOR_ROADTRACKING                = 1,
	    BEHAVIOR_CROSS                       = 2,
	    BEHAVIOR_UTURN                       = 3,
	    BEHAVIOR_LANE_CHANGE                 = 4,
	    BEHAVIOR_STOPLINE_PAUSE              = 5,
	    BEHAVIOR_MAX                         = 6
	} Behavior;
	enum BehaviorProperty
	{
	    INTERSECTION_UNKNOWN = 0,
	    INTERSECTION_LEFT    = 1,
	    INTERSECTION_RIGHT   = 2,
	    INTERSECTION_FORWARD = 3
	} Options;
	struct BehaviorData
	{
		GuidePts_t guidePts[MAX_GUIDE_PTS];
		int num;
	} Data;
	struct timeval timestamp;
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} Decision_t;
/****************************************************
 * Shared Device Meta Data Structures
 */
// GPS Meta Data
#pragma pack (4)
typedef struct MetaNavigation
{
	long int ID;
	double ENU[3];
	double Velocity[3];
	double Eulr[3];
	double AngularVelocity[3];
	double V;
	double A;
	double COV[3];
	double TMbr[4][4];
	double org_xyz[3];
	double GetVelocity()
	{
		return sqrt(pow(Velocity[0], 2) + pow(Velocity[1], 2));
	}
} MetaNavigation_t;
#pragma pack ()

typedef struct MetaLocalNavigation
{
	double T[3][3];
	double UGVtoFCF[3];
	int counter;
} MetaLocalNavigation_t;

typedef struct LUXPoint
{
	float m_x;
	float m_y;
	float m_z;
	float m_dist;
	float m_hAngle;
	float m_vAngle;
	float m_echoWidth;
	unsigned short m_flags;
	unsigned char m_deviceID;
	unsigned char m_channel;
	unsigned char m_subchannel;
	unsigned char m_millisec;
	unsigned short m_microsec;
	unsigned short m_segmentId;
} LUXPoint_t;

// Ibeo(LUX) MetaPoints
typedef struct MetaLUXPoints
{
	float m_startAngle;
	float m_endAngle;
	unsigned int m_scannerStatus;
	unsigned int m_flags;
	unsigned int m_scanNumber;
	unsigned char m_deviceID;
	unsigned char m_scannerType;
	unsigned int m_uPoints;
	LUXPoint_t m_pts[LUX_MAX_POINTS];
} MetaLUXPoints_t;

typedef struct LUXObject
{
	struct LUXPoint2D
	{
		float m_x;
		float m_y;
	};
	enum ObjectClassification
	{
	    Unclassified = 0, ///< = 0: No classification determined (yet)
	    UnknownSmall = 1, ///< = 1: Relatively small structure like pole, tree, single bush, etc. which does not fit to other classes.
	    UnknownBig = 2, ///< = 2: Bigger structure which does not fit other classes.
	    Pedestrian = 3, ///< = 3: Pedestrian, usually determined by moving behaviour.
	    Bike = 4, ///< = 4: bike, motor bike
	    Car = 5, ///< = 5: Passenger car.
	    Truck = 6, ///< = 6: Big vehicle, bus, truck.
	    Structure_Pylon = 7, ///< = 7: Only enhanced, special classification (staf).
	    Structure_Beacon = 8, ///< = 8: Only enhanced, special classification (staf).
	    Structure_GuardRail = 9, ///< = 9: Only enhanced, special classification (staf).
	    Structure_ConcreteBarrier = 10, ///< = 10: Only enhanced, special classification (staf).
	    NumClasses, ///< Number of currently defined classes except Unknown.
	    Unknown = 15 ///< = 15: Compatibility to earlier software which does not determine between UnknownBig or -Small or Unclassified.
	};
	unsigned short m_objectId;
	unsigned short  m_flags; ///< reserved
// Data from the tracking/classification:
	unsigned int  m_objectAge;            ///< number of scans in which this object has been tracked, or instead time?
	unsigned short m_hiddenStatusAge;      ///< Counts how long the object has not been observed but only predicted.
	ObjectClassification m_classification; ///< The object class that is most likely for this object.
	unsigned int  m_classificationAge;           ///< Counts how long the object has been classified in the current classification.
	float   m_classificationQuality;       ///< The quality of the current classification.
	LUXPoint2D m_centerPoint;      ///< Center point of object rectangle, given in Vehicle coordinate system.
	LUXPoint2D m_centerPointSigma;
	float   m_courseAngle;      ///< named by ISO 8855; also called Orientation or Heading [rad]
	float   m_courseAngleSigma; // in [rad]
	LUXPoint2D m_relativeVelocity; ///< Velocity of this object [meter/seconds], relative to the vehicle coordinate system.
	LUXPoint2D m_relativeVelocitySigma;
	LUXPoint2D m_absoluteVelocity; ///< Velocity of this object [meter/seconds] as absolute velocity; the orientation is relative to the vehicle coordinate system.
	LUXPoint2D m_absoluteVelocitySigma;
	LUXPoint2D m_objectBox;         ///< The object's length and width as a rectangle, relative to the object's coordinate system.
	LUXPoint2D m_objectBoxSigma;
	LUXPoint2D m_boundingBoxCenter; ///< Center of the bounding box.
	LUXPoint2D m_boundingBox;       ///< A rectangle in parallel to the vehicle coordinate system (a paraxial rectangle) that contains (bounds) all of this object's points
// These components are also proposed
	LUXPoint2D m_closestPoint; ///< The point of this object that is closest to the origin of the vehicle coordinate system.
// This can also be calculated
	LUXPoint2D m_contourPoints[LUX_MAX_CONTOURPOINTS]; ///< A poly-line that describes the outline of the current object measurement.
	unsigned long long m_vehicleWLANid;    ///< An identifier to be used by WLAN fusion algorithms.
	float m_objectHeight;      ///< The height of this object in [m] (most probably received through WLAN data).
	float m_objectHeightSigma; ///< The standard deviation of the height of this object in [m] (most probably received through WLAN data).
	float m_objectMass; ///< The mass of this object in [kilogram] (as received e.g. through WLAN data)
	/** True, if this object is valid. */
	bool m_isValid;
} LUXObject_t;

// Ibeo(LUX) MetaObjects
typedef struct MetaLUXObjects
{
	unsigned int m_uObjects;
	LUXObject_t obj[LUX_MAX_OBJECTS];
} MetaLUXObjects_t;

// HDL Laser Meta Data
typedef struct LPoint
{
	int x;
	int y;
	int z;
	unsigned short dist;
	unsigned short rot;
	unsigned char i;
	unsigned char c;
} LPoint_t;

typedef struct MetaLaserHdl
{
	int pts_count;
	LPoint_t pts[HDL_MAX_POINT_NUMBER];
} MetaLaserHdl_t;

// Black-White(BW) Camera Meta Data
typedef struct MetaCameraBW
{
	unsigned char mdata[BWCAM_IMAGE_SIZE];
} MetaCameraBW_t;

// Color(C) Camera Meta Data
typedef struct MetaCameraC
{
	unsigned char mdata[CCAM_IMAGE_SIZE];
} MetaCameraC_t;

// Hokuyo Meta Data
//typedef struct MetaHokuyoPoints
//{
//	long range[HOKUYO_MAX_POINTS];
//	long intensity[HOKUYO_MAX_POINTS];
//} MetaHokuyoPoints_t;
//typedef struct MetaHokuyoObjects
//{
//	unsigned int uNumberOfObjects; //Specified the number of objects which were transmitted.
//	unsigned int uSensorStatus; // Specified the status of the Laserscanner. {0: Error and; 1: already}
//
//	unsigned int uaObjectAge[HOKUYO_MAX_OBJECTS]; // Age of an object [Scans]
//	unsigned int uaTrackingNumber[ASL_MAX_OBJECTS]; // For the life of an object constant number.
//
//	/**
//	 * Shows the status of an object.
//	 *
//	 * There are 3 possible states:
//	 *	- 0: known object (the object is older than 1 scan),
//	 *	- 1: unknown object (the object was seen the 1st time) and
//	 *	- 255: the information isn't available.
//	 */
//	unsigned int uaTrackingStatus[HOKUYO_MAX_OBJECTS];
//
//	unsigned int uaNumberOfPoints[HOKUYO_MAX_OBJECTS]; // Number of contour-points which describe the outline of an object.
//
//	double daPointNumberPosX[HOKUYO_MAX_OBJECTS][HOKUYO_MAX_CONTOURPOINTS]; // Relative position in x-direction [m].
//	double daPointNumberPosY[HOKUYO_MAX_OBJECTS][HOKUYO_MAX_CONTOURPOINTS]; // Relative position in Y-direction [m].
//
//	bool baVelocityXValid[HOKUYO_MAX_OBJECTS];
//	double daVelocityX[HOKUYO_MAX_OBJECTS]; // Relative velocity in x-direction [m/s]
//
//	bool baVelocityYValid[HOKUYO_MAX_OBJECTS];
//	double daVelocityY[HOKUYO_MAX_OBJECTS]; // Relative velocity in y-direction [m/s]
//
//	bool baPositionXsigmaValid[HOKUYO_MAX_OBJECTS];
//	double daPositionXsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daPositionX
//
//	bool baPositionYsigmaValid[HOKUYO_MAX_OBJECTS];
//	double daPositionYsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daPositionY
//
//	bool baVelocityXsigmaValid[HOKUYO_MAX_OBJECTS];
//	double daVelocityXsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daVelocityX
//
//	bool baVelocityYsigmaValid[HOKUYO_MAX_OBJECTS];
//	double daVelocityYsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daVelocityY
//
//} MetaHokuyoObjects_t;
typedef struct MetaData
{
	enum
	{
	    META_LASER_HDL      = 1,
	    META_CAMERA_BW      = 2,
	    META_CAMERA_C       = 3,
	    META_NAVIGATION     = 4,
		META_LOCAL_NAVIGATION = 5,
	    META_LUX_POINTS       = 6,
	    META_LUX_OBJECTS    = 7,
//	    META_HOKUYO_POINTS  = 7,
//	    META_HOKUYO_OBJECTS = 8
	} type;
	union
	{
		MetaNavigation_t v_navi;
		MetaLocalNavigation_t v_navi_local;
		MetaLaserHdl_t v_laserHdl;
		MetaCameraBW_t v_cameraBW;
		MetaCameraC v_cameraC;
		MetaLUXPoints v_luxPts;
		MetaLUXObjects v_luxObjs;
//		MetaHokuyoPoints_t v_hokuyoPts;
//		MetaHokuyoObjects_t v_hokuyoObjs;
	} value;
	struct timeval timestamp;
	MetaData()
	{
		SetTimeStamp();
	}
	// set timestamp
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} MetaData_t;
/*****************************************************
 * Shared recognition data structures
 */
//typedef struct RecoSpotLine
//{
//	double ld;
//	double ad;
//	double distance;
//} RecoSpotLine_t;
// stopline
typedef struct RecoStopLine  // RecoType=RT_STOPLINE
{
	double angle;
	double distance;
} RecoStopLine_t;

// Traffic Light
typedef struct RecoTrafficLight  // RecoType=RT_TRAFFICLIGHT
{
	enum TrafficLightType
	{
	    TRL_NONE   = 0,
	    TRL_RED    = 1,
	    TRL_YELLOW = 2,
	    TRL_GREEN  = 3
	} type;
} RecoTrafficLight_t;

// traffic sign 1
typedef struct RecoTrafficSign  // RecoType=RT_TRAFFICSIGN
{
	enum
	{
	    TRS_NO_LEFT_TURN          = 1,
	    TRS_NO_RIGHT_TURN         = 2,
	    TRS_NO_PASS               = 3,
	    TRS_NO_LEFT_RIGHT_TURN    = 4,
	    TRS_NO_LEFT_PASS          = 5,
	    TRS_NO_RIGHT_PASS         = 6,
	    TRS_NO_ENTRY              = 7,
	    TRS_STOP_AVOID            = 8,
	    TRS_SPEED_DOWN            = 9,
	    TRS_ATTENTION_PEDERSTRAIN = 10,
	    TRS_ROAD_UNFLAT           = 11,
	    TRS_EMERGENCY_STOP_AREA   = 12,
	    TRS_STRAIGHT_FORWARD      = 13,
	    TRS_LEFT_TURN             = 14,
	    TRS_RIGHT_TURN            = 15,
	    TRS_STRAIGHT_LEFT         = 16,
	    TRS_STRAIGHT_RIGHT        = 17,
	    TRS_LEFT_RIGHT            = 18,
	    TRS_RIGHT_PASS            = 19,
	    TRS_LEFT_PASS             = 20,
	    TRS_HORN                  = 21,
	    TRS_CIRCLE_DIRVE          = 22,
	    TRS_PEDERSTRAIN_PATH      = 23,
	    TRS_ALLOW_UTURN           = 24,
	    TRS_PARKING_LOT           = 25,
	    TRS_CONE_SIGN             = 26,
	    TRS_DRIVE_TO_LEFT         = 27,
	    TRS_DRIVE_TO_RIGHT        = 28,
	    TRS_UNKNOWNSIGN           = 0
	} type;
	int position;
} RecoTrafficSign_t;

// road tracking (ld&ad)
typedef struct RecoTrackLdAd   // RecoType=RT_TRACK_LDAD
{
	double ld;
	double ad;
	double lanewidth;
	double curbwidth;
	double curvature;
} RecoTrackLdAd_t;

// lux car following & lane change
typedef struct RecoSlowDown
{
	enum
	{
		SL_NONE   = 0,
		SL_FOLLOW = 1,
		SL_CHANGE = 2
	} state;
	double dv;
} RecoSlowDown_t;

// emergcy
typedef struct RecoEmergency
{
	int level;
} RecoEmergency_t;

// obstacle detection
typedef struct RecoSideObs
{
	bool hasLeftObstacle;
	bool hasRightObstacle;
} RecoSideObs_t;

typedef struct RecoData
{
	enum RecoDataType
	{
	    RT_STOPLINE     = 1,
	    RT_TRAFFICSIGN  = 2,
	    RT_TRAFFICLIGHT = 3,
	    RT_TRACK_LDAD   = 4,
		RT_SLOWDOWN     = 5,
		RT_EMERGENCY    = 6,
		RT_SIDEOBS      = 7,
	    RT_MAX          = 8
	} type;
	union RecoDataValue
	{
		RecoStopLine_t v_stopline;
		RecoTrafficSign_t v_ts;
		RecoTrafficLight_t v_tl;
		RecoTrackLdAd_t v_trackLdAd;
		RecoSlowDown_t v_slowdown;
		RecoEmergency_t v_emergency;
		RecoSideObs_t v_sideObs;
	} value;
	struct timeval timestamp;
	double belief;
	RecoData()
	{
		SetTimeStamp();
	}
	// set time stamp
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} RecoData_t;
/*****************************************************
 * Shared Markers
 */
// navigation marker
typedef struct MarkerNavigation   // MarkerType=MARKER_NAVIGATION
{
	int mark;
	double distance;
	double theta;
} MarkerNavi_t;

// intersection state: roadtracking <=> decision
typedef struct MarkerIntersection
{
	bool isFinished;
} MarkerIntersection_t;

// taillight image: HDL => TailLight
//typedef struct MarkerTaillightImage
//{
//	int x;
//	int y;
//	int width;
//	int height;
//} MarkerTaillightImage_t;

// emergency
//typedef struct MarkerObstacle
//{
//	int level;
//} MarkerObstacle_t;

typedef struct MarkerObstacleLux
{
	int level;
} MarkerObstacleLux_t;

// hokuyo->HDL
//typedef struct MarkerHokuyoObs // MarkerType=MARKER_HOKUYO_OBS
//{
//	bool left;
//	bool right;
//	bool back;
//} MarkerHokuyoObs_t;

// velocity dec: ibeo => decision
//typedef struct MarkerVelocityDec
//{
//	double delta_v; // alway be no greater than 0
//	int flag; // 1 = slow down, 0 = do nothing
//} MarkerVelocityDec_t;

typedef struct MarkerVelocityDecLux
{
	double delta_v; // alway be no greater than 0
	int flag; // 1 = slow down, 0 = do nothing
} MarkerVelocityDecLux_t;

// obstacle: track => obstacle
typedef struct MarkerParabola
{
	double a[4];
	double b[4];
	double c[4];
	double belief[4];
} MarkerParabola_t;

typedef struct MarkerLaneChange
{
	enum LaneChangeState
	{
	    LCS_START    = 1,
	    LCS_FINISHED = 2,
	    LCS_RUNNING  = 3,
	    LCS_FAILED   = 4
	} state;

	enum LaneChangeSide
	{
	    LC_LEFT    = 1,
	    LC_RIGHT   = 2,
	    LC_UNKNOWN = 3
	} side;
} MarkerLaneChange_t;

typedef struct MarkerLaneChangeObstacle
{
	bool hasLeftObstacle;
	bool hasRightObstacle;
} MarkerLaneChangeObstacle_t;

typedef struct MarkerTrafficLight
{
	int TrafficLights[10][4];
	int result_length;
	unsigned char pattern[10][5400];
} MarkerTrafficLight_t;

typedef struct MarkerData
{
	enum MarkerDataType
	{
	    MARKER_NAVIGATION          = 1,
	    MARKER_INTERSECTION        = 2,
//	    MARKER_TAILLIGHT_IMAGE 	   = 3,
//	    MARKER_HOKUYO_OBS          = 4,
//	    MARKER_VELOCITY_DEC        = 5,
		MARKER_VELOCITY_DEC_LUX    = 3,
	    MARKER_LANECHANGE          = 4,
	    MARKER_PARABOLA            = 5,
	    MARKER_LANECHANGE_OBSTACLE = 6,
//	    MARKER_LANECHANGE_SIDE     = 10,
//	    MARKER_OBSTACLE            = 11,
		MARKER_OBSTACLE_LUX        = 7,
		MARKER_TL                  = 8,
	    MARKER_MAX                 = 9
	} type;
	union
	{
		MarkerNavi_t v_navi;
		MarkerIntersection_t v_intersection;
//		MarkerTaillightImage_t v_taillightImage;
//		MarkerHokuyoObs_t v_hokuyoobs;
//		MarkerVelocityDec_t v_velocityDec;
		MarkerVelocityDecLux_t v_velocityDecLux;
		MarkerLaneChange_t v_lanechange;
		MarkerParabola_t v_parabola;
		MarkerLaneChangeObstacle_t v_lanechangeobstacle;
//		MarkerLaneChangeSide_t v_lanechangeside;
//		MarkerObstacle_t v_obstacle;
		MarkerObstacleLux_t v_obstacleLux;
		MarkerTrafficLight_t v_tl;
	} value;
} MarkerData_t;
}
#endif // __TYPES_H_
