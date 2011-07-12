#ifndef __TYPES_H_
#define __TYPES_H_

#include <sys/time.h>
#include <stdio.h>
#include <semaphore.h>
#include <vector>

namespace module
{
/****************************************************
 * device meta data macro
 */
// HDL meta point number
#define HDL_LASER_NUMBER 64
#define HDL_MAX_POINT_LASER 4000
#define HDL_MAX_POINT_NUMBER HDL_MAX_POINT_LASER * HDL_LASER_NUMBER
// Black-White(BW) camera meta data size
#define BWCAM_IMAGE_WIDTH 780
#define BWCAM_IMAGE_HEIGHT 582
#define BWCAM_IMAGE_SIZE BWCAM_IMAGE_WIDTH * BWCAM_IMAGE_HEIGHT
// Color(C) camera meta data size
#define CCAM_IMAGE_WIDTH 1292
#define CCAM_IMAGE_HEIGHT 964
#define C_WIDTH 3
#define CCAM_IMAGE_SIZE CCAM_IMAGE_WIDTH * CCAM_IMAGE_HEIGHT * C_WIDTH
// Ibeo(ASL) meta data size
#define ASL_MAX_POINTS 2000
#define ASL_MAX_CONTOURPOINTS	16
#define ASL_MAX_OBJECTS		64
// Hokuyo meta data size
#define HOKUYO_MAX_POINTS 1081
#define HOKUYO_MAX_OBJECTS  20
#define HOKUYO_MAX_CONTOURPOINTS 150


/****************************************************
 * recognition data macro
 */
// pts for tracking recognition (points)
#define TRACK_PTS_NUM 10


/*****************************************************
 * useful enum definition
 */
// Curb tracking type definition
enum CurbTracking
{
	CURB_TRACKING_DEFAULT = 0,
	CURB_TRACKING_LEFT    = 1,
	CURB_TRACKING_RIGHT   = 2,
	CURB_TRACKING_CIRCLE  = 3
};

// intersection point type definition
enum IntersectionPoint
{
	INTER_LEFTTOP     = 1,
	INTER_TOP         = 2,
	INTER_RIGHTTOP    = 3,
	INTER_LEFTMIDDLE  = 4,
	INTER_MIDDLE      = 5,
	INTER_RIGHTMIDDLE = 6,
	INTER_LEFTBOTTOM  = 7,
	INTER_BOTTOM      = 8,
	INTER_RIGHTBOTTOM = 9
};

// obstacle marker (ibeo <--> decision)
//enum ObstacleMarker
//{
//	GUIDANCE_REGION_UNDEFINE = 0,
//	LOCK_GUIDANCE_REGION     = 1,
//	UNLOCK_GUIDANCE_REGION   = 2
//};

// obstacle type for decision (ibeo <--> decision)


/****************************************************
 * Decision Structure (controller <--> decision)
 */
typedef struct Decision
{
	double LDevi;
	double AzimuthDevi;
	double DesiredVelocity;
	double VehicleVelocity;
	char Shift;
	bool Power;
	bool Ignition;
	bool Leftlight;
	bool Rightlight;
	bool Warninglight;
	bool Horn;
	enum
	{
		VS_START        = 1,
		VS_STOP         = 2,
		VS_ROADTRACKING = 3,
		VS_LANECHANGE   = 4,
		VS_INTERSECTION = 5,
		VS_PARKING      = 6,
		VS_PAUSE        = 7,
		VS_OBAVOI       = 8,
		VS_SDRIVE       = 9
	} State;

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
	double COV[3];
	double TMbr[4][4];
	double org_xyz[3];
	struct timeval timestamp;
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} MetaNavigation_t;
#pragma pack ()

// Ibeo(ASL) Point Meta data
typedef struct AslPoint
{
	unsigned short m_uScannerID;
	unsigned short m_uChannel;
	unsigned short m_uSubChannel;
	unsigned short m_uPointStatus;
	double m_dX, m_dY, m_dZ;
	unsigned int m_uEchoPulseWidth;
} AslPoint_t;

// Ibeo(ASL) Meta Data
typedef struct MetaAslPoints
{
	unsigned short m_uScannerType;
	unsigned short m_uECU;
	unsigned int m_uTimestamp;
	double m_dStartAngle;
	double m_dEndAngle;
	unsigned int m_uScanCounter;
	unsigned int m_uPoints;
	AslPoint_t pts[ASL_MAX_POINTS];
} MetaAslPoints_t;

// Ibeo(ASL) Object Meta Data
typedef struct MetaAslObjects
{
	unsigned int uObjectStyle; //Specified the version of the transmitted CANspec.
	unsigned int uNumberOfObjects; //Specified the number of objects which were transmitted.
	unsigned int uSensorStatus; // Specified the status of the Laserscanner. {0: Error and; 1: already}
	unsigned int uCalibrationFlag; // Specified if the Laserscanner (or the IPC) has been calibrated. {0: not calibrated; 1: calibrated}

	bool bCycleCounterIsValid;
	unsigned int uCycleCounter; // Number of the object-dataset (will be incremented with each scan). {uCycleCounter = uScanNumber & 0xFF}
	unsigned int uTimestamp; // Time of the first scanpoint from the corresponding scan.

	bool bSensorDirtyValid;
	/**
	 * Shows if the Laserscanner is dirty or not.
	 *
	 * There are 3 possibilities:
	 *	- 0: The Laserscanner isn't dirty,
	 *	- 1: the Laserscanner is dirty and must been cleaned and
	 *	- 255: the dirt-information isn't available.
	 */
	unsigned int uSensorDirty;

	bool bDirtDetectionStartAngleValid;
	unsigned int uDirtDetectionStartAngle; // Specified the startangle of the dirt-area in case of a dirty Laserscanner [deg].

	bool bDirtDetectionEndAngleValid;
	unsigned int uDirtDetectionEndAngle; // Specified the endangle of the dirt-area in case of a dirty Laserscanner [deg].

	bool bRainDetectionValid;
	/**
	 * Shows if it rains or not.
	 *
	 * There are 3 possibilities:
	 *	- 0: No rain,
	 *	- 1-100: the rain intensity in per-cent
	 *	- 255: the rain-information isn't available.
	 */
	unsigned int uRainDetection;

	bool bRangeOfSightValid;
	unsigned int uRangeOfSight; // This value isn't implementd yet (For future use)!

	unsigned int uaObjectAge[ASL_MAX_OBJECTS]; // Age of an object [Scans]
	unsigned int uaTrackingNumber[ASL_MAX_OBJECTS]; // For the life of an object constant number.
	/**
	 * Shows the status of an object.
	 *
	 * There are 3 possible states:
	 *	- 0: known object (the object is older than 1 scan),
	 *	- 1: unknown object (the object was seen the 1st time) and
	 *	- 255: the information isn't available.
	 */
	unsigned int uaTrackingStatus[ASL_MAX_OBJECTS];

	bool baClassificationCertaintyValid[ASL_MAX_OBJECTS];
	unsigned int uaClassificationCertainty[ASL_MAX_OBJECTS]; // Level of certainty that the classification is correct (0..100%).
	/**
	 * Shows the kind of an object.
	 *
	 * The following kinds of classification are defined:
	 *	- 0: Unknown small (All small objects which do not belong to any of the other classes are listed as <I>Unknown small</I>.)
	 *	- 1: Person (Object meets the criteria of the class <I>Person</I>, like size and velocity.)
	 *	- 2: Car
	 *	- 3: Truck or bus
	 *	- 4: Bike (Object meets the criteria of the class <I>Bike</I>, like size and velocity.)
	 *	- 5: Unknown big l (All big objects which do not belong to any of the other classes are listed as <I>Unknown big</I>.)
	 *	- 6-14: not defined yet
	 *	- 15: Unclassified object.
	 */
	unsigned int uaClass[ASL_MAX_OBJECTS];

	bool baAgeOfClassificationValid[ASL_MAX_OBJECTS];
	unsigned int uaAgeOfClassification[ASL_MAX_OBJECTS]; // Age of the classification in Scans.

	unsigned int uaNumberOfPoints[ASL_MAX_OBJECTS]; // Number of contour-points which describe the outline of an object.
	double daPointNumberPosX[ASL_MAX_OBJECTS][ASL_MAX_CONTOURPOINTS]; // Relative position in x-direction [m].
	double daPointNumberPosY[ASL_MAX_OBJECTS][ASL_MAX_CONTOURPOINTS]; // Relative position in Y-direction [m].

	bool baVelocityXValid[ASL_MAX_OBJECTS];
	double daVelocityX[ASL_MAX_OBJECTS]; // Relative velocity in x-direction [m/s]

	bool baVelocityYValid[ASL_MAX_OBJECTS];
	double daVelocityY[ASL_MAX_OBJECTS]; // Relative velocity in y-direction [m/s]

	bool baAccelerationXValid[ASL_MAX_OBJECTS];
	double daAccelerationX[ASL_MAX_OBJECTS]; // Relative acceleration in x-direction [m/s^2]

	bool baAccelerationYValid[ASL_MAX_OBJECTS];
	double daAccelerationY[ASL_MAX_OBJECTS]; // Relative acceleration in y-direction [m/s^2]

	unsigned int uaRelMomentOfMeasure[ASL_MAX_OBJECTS]; // Moment of measure relative to begin of cycle [ms]

	bool baDetectionProbabilityValid[ASL_MAX_OBJECTS];
	unsigned int uaDetectionProbability[ASL_MAX_OBJECTS]; // Not available yet. Reserved - for future used.

	bool baPositionXsigmaValid[ASL_MAX_OBJECTS];
	double daPositionXsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daPositionX

	bool baPositionYsigmaValid[ASL_MAX_OBJECTS];
	double daPositionYsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daPositionY

	bool baVelocityXsigmaValid[ASL_MAX_OBJECTS];
	double daVelocityXsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daVelocityX

	bool baVelocityYsigmaValid[ASL_MAX_OBJECTS];
	double daVelocityYsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daVelocityY

	bool baAccelerationXsigmaValid[ASL_MAX_OBJECTS];
	double daAccelerationXsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daAccelerationX

	bool baAccelerationYsigmaValid[ASL_MAX_OBJECTS];
	double daAccelerationYsigma[ASL_MAX_OBJECTS]; // Standard deviation of value daAccelerationY

	bool baPositionCorrelCoefficientValid[ASL_MAX_OBJECTS];
	double daPositionCorrelCoefficient[ASL_MAX_OBJECTS]; // Not available yet. Reserved - for future used.

	bool baVelocityCorrelCoefficientValid[ASL_MAX_OBJECTS];
	double daVelocityCorrelCoefficient[ASL_MAX_OBJECTS]; // Not available yet. Reserved - for future used.

	bool baAccelerationCorrelCoefficientValid[ASL_MAX_OBJECTS];
	double daAccelerationCorrelCoefficient[ASL_MAX_OBJECTS]; // Not available yet. Reserved - for future used.

	bool baHeightValid[ASL_MAX_OBJECTS];
	double daHeight[ASL_MAX_OBJECTS]; // Height of the object [m].

	bool baHeightSigmaValid[ASL_MAX_OBJECTS];
	double daHeightSigma[ASL_MAX_OBJECTS]; // Standard deviation of the value daHeight.

	bool baACCTypeValid[ASL_MAX_OBJECTS];
	unsigned int uaACCType[ASL_MAX_OBJECTS]; // Characterizes the object type for ACC applicatins

	bool baLaneValid[ASL_MAX_OBJECTS];
	unsigned int uaLane[ASL_MAX_OBJECTS]; // Position of the object with respect to our lane. Only available in certain configurations.

	bool baTimeToCollisionValid[ASL_MAX_OBJECTS];
	unsigned int uaTimeToCollision[ASL_MAX_OBJECTS]; // Time to a collision between the specified object and the ego-car.

	bool baCrashProbabilityValid[ASL_MAX_OBJECTS];
	unsigned int uaCrashProbability[ASL_MAX_OBJECTS]; // Probability of a crash between the specified object and the ego-car.

	bool baPointTypeValid[ASL_MAX_OBJECTS];
	/**
	 * The point type apply to the corresponding contour point.
	 *
	 * They are coded as follows (in binary representation, MSBit first):
	 *	- ASL_POINT_NOT_COVERED : Not closest point (all but one point have this attribute)
	 *	- ASL_POINT_LEFT_COVERED : Left side point due to occlusion by another object
	 *	- ASL_POINT_RIGHT_COVERED : Right side point due to occlusion by another object
	 *	- ASL_POINT_RIGHT_SCANAREABORDER : Side point due to borders of the field of vision
	 *	- ASL_POINT_LEFT_SCANAREABORDER : Side point due to borders of the field of vision
	 *	- ASL_POINT_CLOSEST : Closest point (Only one point has this attribute)
	 *
	 * Depend on the used CANspec it's impossible to differ is it an left- or right covered scanborder.
	 * If so, a point will be set with the flag ASL_POINT_RIGHT_SCANAREABORDER and ASL_POINT_LEFT_SCANAREABORDER.
	 */
	char caPointType[ASL_MAX_OBJECTS][ASL_MAX_CONTOURPOINTS];


	bool bLeftLaneOffsetValid;
	double dLeftLaneOffset; // Offset in [m] between the Reference point and the left lane boundary, at the x-position of the reference point.

	bool bRightLaneOffsetValid;
	double dRightLaneOffset; // Offset in [m] between the Reference point and the right lane boundary, at the x-position of the reference point.

	bool bHeadingAngleValid;
	double dHeadingAngle; // The basic heading of the Lane in [rad].

	bool bCurvatureValid;
	double dCurvature; // The curvature of the road, in [1/m].

	bool bNumberOfLanesValid;
	unsigned int uNumberOfLanes; // Indicates the number of detected road lanes.

	bool bConfidenceLevelValid;
	unsigned int uConfidenceLevel; // The level of confidence of the current Lane Detection parameters.

	bool bViewRangeValid;
	double dViewRange; // Forward view range of the Lane detection in [m].

	unsigned int uErrorCode; // Shows the Laserscanner-specific error/status code.
	unsigned int uSensorIdentifier; // Shows the CAN-identifier of the corresponding Laserscanner.

	bool bChecksumIsValid;
	unsigned int uChecksum; // Shows the checksum transmitted over CAN.

	unsigned int uCP_ObjectNumber; // Number of the crash object (CrashPrediction application)
	bool bCP_ObjectNumberValid;

	unsigned int uCP_TimeToCollision; // Time to collision in [ms] of the crash object
	bool bCP_TTC_Valid;

	unsigned int uCP_CrashProbability; // probability of an crash with the Crash-Object in [%]
	bool bCP_CrashProbability_Valid;
} MetaAslObjects_t;

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
	LPoint_t pts[HDL_MAX_POINT_NUMBER];
	int pts_count;
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
typedef struct MetaHokuyoPoints
{
	long range[HOKUYO_MAX_POINTS];
	long intensity[HOKUYO_MAX_POINTS];
} MetaHokuyoPoints_t;

typedef struct MetaHokuyoObjects
{
	unsigned int uNumberOfObjects; //Specified the number of objects which were transmitted.
	unsigned int uSensorStatus; // Specified the status of the Laserscanner. {0: Error and; 1: already}

	unsigned int uaObjectAge[HOKUYO_MAX_OBJECTS]; // Age of an object [Scans]
	unsigned int uaTrackingNumber[ASL_MAX_OBJECTS]; // For the life of an object constant number.

	/**
	 * Shows the status of an object.
	 *
	 * There are 3 possible states:
	 *	- 0: known object (the object is older than 1 scan),
	 *	- 1: unknown object (the object was seen the 1st time) and
	 *	- 255: the information isn't available.
	 */
	unsigned int uaTrackingStatus[HOKUYO_MAX_OBJECTS];

	unsigned int uaNumberOfPoints[HOKUYO_MAX_OBJECTS]; // Number of contour-points which describe the outline of an object.

	double daPointNumberPosX[HOKUYO_MAX_OBJECTS][HOKUYO_MAX_CONTOURPOINTS]; // Relative position in x-direction [m].
	double daPointNumberPosY[HOKUYO_MAX_OBJECTS][HOKUYO_MAX_CONTOURPOINTS]; // Relative position in Y-direction [m].

	bool baVelocityXValid[HOKUYO_MAX_OBJECTS];
	double daVelocityX[HOKUYO_MAX_OBJECTS]; // Relative velocity in x-direction [m/s]

	bool baVelocityYValid[HOKUYO_MAX_OBJECTS];
	double daVelocityY[HOKUYO_MAX_OBJECTS]; // Relative velocity in y-direction [m/s]

	bool baPositionXsigmaValid[HOKUYO_MAX_OBJECTS];
	double daPositionXsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daPositionX

	bool baPositionYsigmaValid[HOKUYO_MAX_OBJECTS];
	double daPositionYsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daPositionY

	bool baVelocityXsigmaValid[HOKUYO_MAX_OBJECTS];
	double daVelocityXsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daVelocityX

	bool baVelocityYsigmaValid[HOKUYO_MAX_OBJECTS];
	double daVelocityYsigma[HOKUYO_MAX_OBJECTS]; // Standard deviation of value daVelocityY

} MetaHokuyoObjects_t;

typedef struct MetaData
{
	enum
	{
		META_LASER_HDL      = 1,
		META_CAMERA_BW      = 2,
		META_CAMERA_C       = 3,
		META_NAVIGATION     = 4,
		META_ASL_POINTS     = 5,
		META_ASL_OBJECTS    = 6,
		META_HOKUYO_POINTS  = 7,
		META_HOKUYO_OBJECTS = 8
	} type;

	union
	{
		MetaNavigation_t v_navi;
		MetaLaserHdl_t v_laserHdl;
		MetaCameraBW_t v_cameraBW;
		MetaCameraC_t v_cameraC;
		MetaAslPoints_t v_aslPts;
		MetaAslObjects_t v_aslObjs;
		MetaHokuyoPoints_t v_hokuyoPts;
		MetaHokuyoObjects_t v_hokuyoObjs;
	} value;
	struct timeval timestamp;

	// set timestamp
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} MetaData_t;


/*****************************************************
 * Shared recognition data structures
 */

// intersection
typedef struct IntersectionRoute
{
	int len;
	double east[100];
	double north[100];
	bool IsValid;
} IntersectionRoute_t;

typedef struct RecoIntersection // RecoType=RT_INTERSECTION
{
	IntersectionRoute_t currentroute;
	IntersectionRoute_t leftroute;
	IntersectionRoute_t middleroute;
	IntersectionRoute_t rightroute;
} RecoIntersection_t;

// spot
typedef struct RecoSpot  // RecoType=RT_SPOT
{
	int len;
	double east[50];
	double north[50];
} RecoSpot_t;

// obstacle
typedef struct RecoObstacle  // RecoType=RT_OBSTACLE
{
	int type; // 0-stop; 1-lanechange;
	int len;
	double east[50];
	double north[50];
} RecoObstacle_t;

// stopline
typedef struct RecoStopLine  // RecoType=RT_STOPLINE
{
	double angle;
	double distance;
} RecoStopLine_t;

// Traffic Light
typedef struct RecoTrafficLight  // RecoType=RT_TRAFFICLIGHT
{
	int type;
} RecoTrafficLight_t;

// traffic sign 1
typedef struct RecoTrafficSign  // RecoType=RT_TRAFFICSIGN
{
	enum
	{
		TR_NO_LEFT_TURN          = 1,
		TR_NO_RIGHT_TURN         = 2,
		TR_NO_PASS               = 3,
		TR_NO_LEFT_RIGHT_TURN    = 4,
		TR_NO_LEFT_PASS          = 5,
		TR_NO_RIGHT_PASS         = 6,
		TR_NO_ENTRY              = 7,
		TR_STOP_AVOID            = 8,
		TR_SPEED_DOWN            = 9,
		TR_ATTENTION_PEDERSTRAIN = 10,
		TR_ROAD_UNFLAT           = 11,
		TR_EMERGENCY_STOP_AREA   = 12,
		TR_STRAIGHT_FORWARD      = 13,
		TR_LEFT_TURN             = 14,
		TR_RIGHT_TURN            = 15,
		TR_STRAIGHT_LEFT         = 16,
		TR_STRAIGHT_RIGHT        = 17,
		TR_LEFT_RIGHT            = 18,
		TR_RIGHT_PASS            = 19,
		TR_LEFT_PASS             = 20,
		TR_HORN                  = 21,
		TR_CIRCLE_DIRVE          = 22,
		TR_PEDERSTRAIN_PATH      = 23,
		TR_ALLOW_UTURN           = 24,
		TR_PARKING_LOT           = 25,
		TR_CONE_SIGN             = 26,
		TR_DRIVE_TO_LEFT         = 27,
		TR_DRIVE_TO_RIGHT        = 28,
		TR_GREEN                 = 29,
		TR_YELLOW                = 30,
		TR_RED                   = 31,
		TR_UNKNOWNSIGN           = 0
	} type;

	int position;
} RecoTrafficSign_t;

// road tracking (ld&ad)
typedef struct RecoTrackLdAd   // RecoType=RT_TRACK_LDAD
{
	double ld;
	double ad;
	double lanewidth;
	double curvature;
} RecoTrackLdAd_t;

// road tracking (points)
typedef struct RecoTrackPts // RecoType=RT_TRACK_PTS
{
	double x[TRACK_PTS_NUM];
	double y[TRACK_PTS_NUM];
	double width;
	double curvature;
} RecoTrackPts_t;

// road tracking (ld ad & points)
typedef struct RecoTrackRoad //RecoType=RT_TRACK_ROAD
{
	double ld;
	double ad;
	double x[TRACK_PTS_NUM];
	double y[TRACK_PTS_NUM];
	double lanewidth;
	double curvature;
} RecoTrackRoad_t;

typedef struct RecoData
{
	enum
	{
		RT_OBSTACLE     = 1,
		RT_STOPLINE     = 2,
		RT_TRAFFICSIGN  = 3,
		RT_TRAFFICLIGHT = 4,
		RT_INTERSECTION = 5,
		RT_SPOT         = 6,
		RT_TRACK_LDAD   = 7,
		RT_TRACK_PTS    = 8,
		RT_TRACK_ROAD   = 9
	} type;

	union
	{
		RecoObstacle_t v_obstacle;
		RecoStopLine_t v_stopline;
		RecoTrafficSign_t v_ts;
		RecoTrafficLight_t v_tl;
		RecoIntersection_t v_intersection;
		RecoTrackLdAd_t v_trackLdAd;
		RecoTrackPts_t v_trackPts;
		RecoTrackRoad_t v_trackRoad;
		RecoSpot_t v_spot;
	} value;
	struct timeval timestamp;
	double belief;

	// set time stamp
	void SetTimeStamp()
	{
		gettimeofday(&timestamp, NULL);
	}
} RecoData_t;


/*****************************************************
 * Shared RNDF Structures
 */

// rndf intersection
typedef struct RoutePoint
{
	int len;
	double east[100];
	double north[100];
	bool IsValid;
} RoutePoint_t;

typedef struct RndfIntersection // RndfType=RNDF_INTERSECTION
{
	RoutePoint_t leftroute;    //左拐路线
	RoutePoint_t middleroute;  //直行路线
	RoutePoint_t rightroute;   //右拐路线
	RoutePoint_t utrunroute;   //U型路线
} RndfIntersection_t;

typedef struct RndfData
{
	enum
	{
		RNDF_INTERSECTION = 1
	} type;

	union
	{
		RndfIntersection_t v_intersection;
	} value;
} RndfData_t;


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

// intersection state
typedef struct MarkerIntersection
{
	bool IsLeftPass;
	bool IsRightPass;
	bool IsMiddlePass;
	int flag;
	bool isFinished;
	bool isIntersection;
} MarkerIntersection_t;

// taillight image: HDL => TailLight
typedef struct MarkerTaillightImage
{
	unsigned char imgdata[CCAM_IMAGE_SIZE];
	int width;
	int height;
} MarkerTaillightImage_t;

// ibeo <=> decision
typedef struct MarkerObstacle
{
	enum
	{
		OBSTACLE_STATE_AVOID   = 1,
		OBSTACLE_STATE_STOP    = 2,
		OBSTACLE_STATE_RECOVER = 3
	} marker;
} MarkerObstacle_t;

// hokuyo->HDL
typedef struct MarkerHokuyoObs // MarkerType=MARKER_HOKUYO_OBS
{
	bool left;
	bool right;
	bool back;
} MarkerHokuyoObs_t;

// car following: ibeo => decision
typedef struct MarkerCarFollowing
{
	double xi[6];
	double yi[6];
	double vi[6];
} MarkerCarFollowing_t;

// velocity dec: ibeo => decision
typedef struct MarkerVelocityDec
{
	double velocity;
	double distance;
	double angle;
	bool isDo;
} MarkerVelocityDec_t;

typedef struct MarkerData
{
	enum
	{
		MARKER_NAVIGATION      = 1,
		MARKER_INTERSECTION    = 2,
		MARKER_TAILLIGHT_IMAGE = 3,
		MARKER_OBSTACLE        = 4,
		MARKER_HOKUYO_OBS      = 5,
		MARKER_CAR_FOLLOWING   = 6,
		MARKER_VELOCITY_DEC    = 7
	} type;

	union
	{
		MarkerNavi_t v_navi;
		MarkerIntersection_t v_intersection;
		MarkerTaillightImage_t v_taillightImage;
		MarkerObstacle_t v_obstacle;
		MarkerHokuyoObs_t v_hokuyoobs;
		MarkerCarFollowing_t v_carFollowing;
		MarkerVelocityDec_t v_velocityDec;
	} value;
} MarkerData_t;
}

#endif // __TYPES_H_
