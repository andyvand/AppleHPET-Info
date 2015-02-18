/*
 * Apple HPET driver C++ class
 *
 * Reverse engineered by AnV Software
 * For use in our drivers...
 */

#include <libkern/OSTypes.h>
#include <IOKit/IOLib.h>

/* Defines (Apple) */
#define kSetVectorPhysicalDestination "SetVectorPhysicalDestination"

/* HPET Flags (Apple) */
#define hpetAddr 0xFED00000 /* HPET timer address */
#define hptcAE 0x80 /* HPET config - memory enable */
#define HPET_REQFL_64BIT 0x00000001	/* Timer is 64 bits */
#define TIM_CONF 0

/* Flags (Other) */
#define HPET_TIMER_STRIDE 0x20 /* Stride between sets of HPET Timer Registers */

/* enumerators */
typedef enum RegisterOffsets {
    /* HPET General Register Offsets */
    HPET_GENERAL_CAPABILITIES_ID_OFFSET=0x000,
    HPET_GENERAL_CONFIGURATION_OFFSET=0x010,
    HPET_GENERAL_INTERRUPT_STATUS_OFFSET=0x020,
    /* HPET Timer Register Offsets */
    HPET_MAIN_COUNTER_OFFSET=0x0F0,
    HPET_TIMER_CONFIGURATION_OFFSET=0x100,
    HPET_TIMER_COMPARATOR_OFFSET=0x108,
    HPET_TIMER_MSI_ROUTE_OFFSET=0x110
} RegisterOffsets_t;

typedef enum ConfigReg {
    /* Reserved0  (0x0000000000000001ULL) */
    Tn_LVL_TRG_INT=0x0000000000000002ULL, /* LevelTriggeredInterrupt */
    Tn_INT_ENB_CNF=0x0000000000000004ULL, /* InterruptEnable */
    TIM_COMP=0x0000000000000008ULL, /* PeriodicInterruptEnable */
    Tn_INT_CAP_CNF=0x0000000000000010ULL, /* PeriodicInterruptCapablity */
    Tn_CNT_CAP_SIZ=0x0000000000000020ULL, /* CounterSizeCapablity */
    Tn_VAL_ENB_SET=0x0000000000000040ULL, /* ValueSetEnable */
    /* Reserved1  (0x0000000000000080ULL) */
    Tn_CNT_ENB_SIZ=0x0000000000000100ULL, /* CounterSizeEnable */
    Tn_INT_RTE_MSK=0x0000000000003E00ULL, /* InterruptRoute (Mask) */
    Tn_INT_ENB_MSI=0x0000000000004000ULL, /* MsiInterruptEnable */
    Tn_INT_MSI_CAP=0x0000000000008000ULL, /* MsiInterruptCapablity */
    /* Reserved2  (0x00000000FFFF0000ULL) */
    Tn_INT_RTE_CAP=0xFFFFFFFF00000000ULL /* InterruptRouteCapability */
} ConfigReg_t;

/* Structures and unions */
typedef union hpetGeneralCapabilitiesIDRegister { /* HPET General Capabilities and ID Register */
    struct Bits {
        uint32_t Revision:8;
        uint32_t NumberOfTimers:5;
        uint32_t CounterSize:1;
        uint32_t Reserved0:1;
        uint32_t LegacyRoute:1;
        uint32_t VendorId:16;
        uint32_t CounterClockPeriod:32;
    } B;
    uint64_t U64;
} hpetGeneralCapabilitiesIDRegister_t;

typedef union hpetGeneralConfigurationRegister { /* HPET General Configuration Register */
    struct Bits {
        uint32_t  MainCounterEnable:1;
        uint32_t  LegacyRouteEnable:1;
        uint32_t  Reserved0:30;
        uint32_t  Reserved1:32;
    } B;
    uint64_t U64;
} hpetGeneralConfigurationRegister_t;

typedef union hpetTimerConfigurationRegister { /* HPET Timer Configuration Register */
    struct Bits {
        uint32_t Reserved0:1;
        uint32_t LevelTriggeredInterrupt:1;
        uint32_t InterruptEnable:1;
        uint32_t PeriodicInterruptEnable:1;
        uint32_t PeriodicInterruptCapablity:1;
        uint32_t CounterSizeCapablity:1;
        uint32_t ValueSetEnable:1;
        uint32_t Reserved1:1;
        uint32_t CounterSizeEnable:1;
        uint32_t InterruptRoute:5;
        uint32_t MsiInterruptEnable:1;
        uint32_t MsiInterruptCapablity:1;
        uint32_t Reserved2:16;
        uint32_t InterruptRouteCapability;
    } B;
    uint64_t U64;
} hpetTimerConfigurationRegister_t;

typedef union hpetTimerMSIRouteRegister { /* HPET Timer MSI Route Register */
    struct Bits {
        uint32_t Value:32;
        uint32_t Address:32;
    } B;
    uint64_t U64;
} hpetTimerMSIRouteRegister;

typedef struct hpetReg { /* Memory mapped registers for the HPET */
    /* General capabilities */
    hpetGeneralCapabilitiesIDRegister_t	GCAP_ID;

    /* Reserved 1 */
    uint64_t rsv1;

    /* General configuration */
    hpetGeneralConfigurationRegister_t GEN_CONF;

    /* Reserved 2 */
    uint64_t rsv2;

    /* General Interrupt status */
    uint64_t GINTR_STA;

    /* Reserved 3 */
    uint64_t rsv3[25];

    /* Main counter */
    uint64_t MAIN_CNT;

    /* Reserved 4 */
    uint64_t rsv4;

    /* Timer 0 config and cap */
    hpetTimerConfigurationRegister_t TIM0_CONF;
    /* Timer 0 comparator */
    uint64_t TIM0_COMP;

    /* Reserved 5 */
    uint64_t rsv5[2];

    /* Timer 1 config and cap */
    hpetTimerConfigurationRegister_t TIM1_CONF;
    /* Timer 1 comparator */
    uint64_t TIM1_COMP;

    /* Reserved 6 */
    uint64_t rsv6[2];

    /* Timer 2 config and cap */
    hpetTimerConfigurationRegister_t TIM2_CONF;
    /* Timer 2 comparator */
    uint64_t TIM2_COMP;

    /* Reserved 7 */
    uint64_t rsv7[2];
} hpetReg, hpetReg_t;

typedef struct hpetTimer {
    hpetTimerConfigurationRegister_t Config; /* Timer config and capabilities */
    uint64_t Compare; /* Timer comparitor */
} hpetTimer_t, HpetTimer;

typedef struct hpetInfo
{
    uint64_t	hpetCvtt2n;
    uint64_t	hpetCvtn2t;
    uint64_t	tsc2hpet;
    uint64_t	hpet2tsc;
    uint64_t	bus2hpet;
    uint64_t	hpet2bus;
    uint32_t	rcbaArea;
    uint32_t	rcbaAreap;
} hpetInfo_t;

typedef struct hpetRequest
{
    uint32_t	flags;
    uint32_t	hpetOffset;
    uint32_t	hpetVector;
} hpetRequest_t;

/* Kernel methods */
extern uint64_t hpetFemto;
extern uint64_t hpetFreq;
extern uint64_t hpetCvtt2n;
extern uint64_t hpetCvtn2t;
extern uint64_t tsc2hpet;
extern uint64_t hpet2tsc;
extern uint64_t bus2hpet;
extern uint64_t hpet2bus;
extern vm_offset_t rcbaArea;
extern uint32_t rcbaAreap;
extern void map_rcbaAread(void);
extern void hpet_init(void);
extern void hpet_save(void);
extern void hpet_restore(void);
extern int hpet_register_callback(int (*hpet_reqst)(uint32_t apicid, void *arg, hpetRequest_t *hpet), void *arg);
extern int hpet_request(uint32_t cpu);
extern uint64_t rdHPET(void);
extern void hpet_get_info(hpetInfo_t *info);

#ifdef XNU_KERNEL_PRIVATE
extern int HPETInterrupt(void);
#endif /* XNU_KERNEL_PRIVATE */

/* Expansion data */
#define ErrataTable ExpansionData->errata

/* AppleHPET kext class */
class AppleHPET : IOACPIPlatformDevice
{
    OSDeclareDefaultStructors(AppleHPET)

protected:
    /* Expansion data */
    typedef struct {
        uint64_t errata;
    } ExpansionData;
    ExpansionData *expansionData;

public:
    /* Publish method (IOServiceMatchingNotificationHandler) */
    bool cpuDriverPublished(void *target = this, void *refCon, IOService *newService, IONotifier *notifier);

    /* Standard IOService methods */
    virtual bool start(IOService *provider);
    virtual void stop(IOService *service);
    virtual void free(void);

    /* Probe HPET device */
    virtual bool probeHPET(void);

    /* HPET save and restore */
    virtual void *hpetSaveState(void);
    virtual void *hpetRestoreState(void);

    /* Configure timer method */
    virtual IOReturn configureTimer(HpetTimer *timer);

    /* Timer handling methods */
    virtual IOReturn hpetTimerAcquire(uint32_t a1, void (*a2)(), uint32_t *a3, uint32_t *a4, void **a5);
    virtual IOReturn hpetTimerRelease(uint32_t target);
    virtual IOReturn hpetTimerCommand(uint32_t a1, uint32_t a2);

    /* Interrrupt methods */
    virtual IOReturn setInterruptMessageDestination(HpetTimer const *timer, uint32_t destination);
    virtual uint32_t getInterruptVectorAtIndex(uint32_t index);
    virtual IOReturn getInterruptControllerAtIndex(uint32_t index);
};
