//parameter
//wang hao 2013.12.28     ver1.0

#define SAMPLE         2048
#define iteration      10000/*観測回数*/
#define trial 10
#define PU_Power       10  /*PUの送信電力10dBm*/
#define MIN_DIST       5
#define MAX_DIST       100
#define AVERAGE_ON  100000.0      //average on time  = 100[ms]
#define AVERAGE_OFF 400000.0      //average off time = 100[ms]
#define Ts 5

/*noise*/
#define NOISE -103.0103           /*I相Q相の雑音電力-103.0103dBm*/
#define NOISE_sinti 0.00000000005 /*I相Q相の雑音電力の真値*/
#define NOISE_SINTI 0.0000000001  /*雑音電力真値*/
#define NOISE_POWER -100         /*雑音電力 -100dBm*/

/*path loss*/
#define GT 2.0                   /*gain(dBi)*/
#define GR 2.0                   /*gain(dBi)*/
#define expon 3.0                /*path-loss exponent*/
#define ref_dis 1               /*reference distance* 1 (m)*/


/*PU parameter*/
#define PU_x 0              /*PUのx座標*/
#define PU_y 0              /*PUのy座標*/
#define Frequency 2400000000     /*周波数:2.4Ghz*/

#define length 2048          /*PUの送信時間*/
#define Point_transition 1536  /*遷移点*/

/*generation mode*/
#define ON_OFF_MODE 1
#define OFF_ON_MODE 0
#define ON_OFF_ON_MODE 0
#define OFF_ON_OFF_MODE 0

/*sensor parameter*/
#define sensor_number   20  /*センサの数*/
#define coop_sensor_number  20/*協調センサ数*/
#define sensor_range_x 50     /*センサーの配置範囲15(m)　x軸*/
#define sensor_range_y 50     /*センサーの配置範囲15(m)　y軸*/
#define sensor_time    101  /*観測時間*/
#define sampling_rate  5       /*サンプリング周期 5(micro second）->200kHz*/
#define sample_number  2048    /*一回収集するpoint数*/
#define kaisu          1     /*観測回数*/
#define rest           1000    /*センサの待機時間*/
#define P_FA           0.01 /*誤警報確率*/
#define MAX_number     1     /*誤警報確率による間違いのカット数*/
#define Velocity       20     /*センサの移動速度 20(km/h)*/
#define squzze_size 5
#define coop_range  20/*cooperative range*/

/*MODE CHANGE*/
#define test_MODE 1
#define database_MODE 1
#define single_sensing_MODE 0
#define weight_MODE 1
#define cooperative_MODE 0
#define filter_MODE 1
#define transition_SAMPLE_MODE 1
/*evaluation*/
#define POWER_ERROR_MODE 0
#define TIME_ERROR_MODE 0
#define transition_average_MODE  0 /*1->0*/
#define transition_distribution_MODE 0
#define transition_correct_MODE 1 /*0->1*/
#define MN_MODE 1

/*database*/
#define hard_AND_thershold 10    /*硬判定ANDルール閾値*/

/*fading parameter*/
#define wave_number     15        /*素波数*/
#define c 300000000               /*光速度*/
#define kizami 1000000          /*刻み：1 micro second*/
/*shadowing parameter*/
#define SHAD_SIGMA 8.0
/*KS test*/
#define EPS1 0.001
#define EPS2  1.0e-8
