/*
	SI4735 properties
*/

#ifndef __SI4735_PROPERTIES__
#define __SI4735_PROPERTIES__



// ___ Common Properties ___________________________________________________________________________________________

/*
	GPO IEN

	Configures the sources for the GPO2/INT interrupt pin. Valid sources are the lower 8 bits of the STATUS byte,
	including CTS, ERR, RSQINT, RDSINT (Si4705/21/31/35/37/39 only), and STCINT bits. The corresponding bit is
	set before the interrupt occurs. The CTS bit (and optional interrupt) is set when it is safe to send the next
	command. The CTS interrupt enable (CTSIEN) can be set with this property and the POWER_UP command. The
	state of the CTSIEN bit set during the POWER_UP command can be read by reading this property and modified by
	writing this property.

	Default: 0x0000
*/

// property
#define SI4735_GPO_IEN 0x0001
// values
#define SI4735_RSQREP	0x0800	// Interrupt generated even if RSQINT is already set.
#define SI4735_RDSREP	0x0400	// Interrupt generated even if RDSINT is already set.
#define SI4735_STCREP	0x0100	// Interrupt generated even if STCINT is already set.
#define SI4735_CTSIEN	0x0080	// Interrupt generated when CTS is set.
#define SI4735_ERRIEN	0x0040	// Interrupt generated when ERR is set.
#define SI4735_RSQIEN	0x0008	// Interrupt generated when RSQINT is set.
#define SI4735_RDSIEN	0x0004	// Interrupt generated when RDSINT is set.
#define SI4735_STCIEN	0x0001	// Interrupt generated when STCINT is set.




/*
	DIGITAL OUTPUT FORMAT

	(Si4705/06/21/31/35/37/39/41/43/45/84/85 Only)

	Configures the digital audio output format. Configuration options include
	DCLK edge, data format, force mono, and sample precision.

	Default: 0x0000
*/

// property
#define SI4735_DIGITAL_OUTPUT_FORMAT	0x0102	// Configures the digital audio output format.
// values
#define SI4735_DCLK_RISING_EDGE		0x0000	// DCLK rising edge.
#define SI4735_DCLK_FALLING_EDGE	0x0080	// DCLK rising edge.
#define SI4735_I2S			0x0000
#define SI4735_LEFT_JUSTIFIED		0x0030
#define SI4735_MSB_SECOND_CLK		0x0040	// MSB at second DCLK after DFS pulse.
#define SI4735_MSB_FIRST_CLK		0x0060	// MSB at first DCLK after DFS pulse.
#define SI4735_DSTEREO			0x0000	// Stereo (blend).       
#define SI4735_DMONO			0x0004	// Force mono.       
#define SI4735_D16BITS			0x0000	// 16 bits per audio sample.
#define SI4735_D20BITS			0x0001	// 20 bits per audio sample.
#define SI4735_D24BITS			0x0002	// 24 bits per audio sample.
#define SI4735_D8BITS			0x0003	// 16 bits per audio sample.




/*
    DIGITAL OUTPUT SAMPLE RATE

    (Si4705/06/21/31/35/37/39/41/43/45/84/85 Only)

    Enables digital audio output and configures digital audio output sample rate in samples per second (sps). When
    DOSR[15:0] is 0, digital audio output is disabled. The over-sampling rate must be set in order to satisfy a minimum
    DCLK of 1 MHz. To enable digital audio output, program DOSR[15:0] with the sample rate in samples per second.
    The system controller must establish DCLK and DFS prior to enabling the digital audio output else the
    device will not respond and will require reset. The sample rate must be set to 0 before the DCLK/DFS is
    removed.

    Note: DIGITAL_OUPTUT_SAMPLE_RATE is supported in FM receive component 2.0 or higher.

    Default: 0x0000 (digital audio output disabled)

    Units: sps

    Range: 32–48 ksps, 0 to disable digital audio output
*/

// property
#define SI4735_DIGITAL_OUTPUT_SAMPLE_RATE 0x0104




/*
    REFCLK FREQ

    Sets the frequency of the REFCLK from the output of the prescaler. The REFCLK range is 31130 to 34406 Hz
    (32768 ±5% Hz) in 1 Hz steps, or 0 (to disable AFC). For example, an RCLK of 13 MHz would require a prescaler
    value of 400 to divide it to 32500 Hz REFCLK. The reference clock frequency property would then need to be set to
    32500 Hz. RCLK frequencies between 31130 Hz and 40 MHz are supported, however, there are gaps in frequency
    coverage for prescaler values ranging from 1 to 10, or frequencies up to 311300 Hz.

    Default: 0x8000 (32768)
    Units: 1 Hz
    Step: 1 Hz  delay(_100ms_);

    Range: 31130–34406
*/

// property
#define SI4735_REFCLK_FREQ 0x0201
// values
#define SI4735_AFC_DISABLE 0x0000    // Disable the AFC




/*
    REFCLK PRESCALE

    Sets the number used by the prescaler to divide the external RCLK down to the internal REFCLK. The range may
    be between 1 and 4095 in 1 unit steps. For example, an RCLK of 13 MHz would require a prescaler value of 400 to
    divide it to 32500 Hz. The reference clock frequency property would then need to be set to 32500 Hz. The RCLK
    must be valid 10 ns before sending and 20 ns after completing the FM_TUNE_FREQ and FM_TUNE_START
    commands. In addition, the RCLK must be valid at all times for proper AFC operation. The RCLK may be removed
    or reconfigured at other times. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 1.

    Default: 0x0001
    Step: 1
    Range: 1–4095
*/

// property
#define SI4735_REFCLK_PRESCALE 0x0202 
// values
#define SI4735_RCLK 0x0000           // RCLK pin is clock source.
#define SI4735_DCLK 0x1000           // DCLK pin is clock source.
#define SI4735_AFC_DISABLE 0x0000    // Disable the AFC




/*
    RX VOLUME

    (Not applicable to Si4749)

    Sets the audio output volume. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 63.

    Default: 0x003F
    Step: 1
    Range: 0–63
*/

// property
#define SI4735_RX_VOLUME 0x4000




/*
    RX HARD MUTE

    (Not applicable to Si4749)

    Mutes the audio output. L and R audio outputs may be muted independently. The CTS bit (and optional interrupt) is
    set when it is safe to send the next command. This property may only be set or read when in powerup mode. The
    default is unmute (0x0000).

    Default: 0x0000
*/

// property
#define SI4735_RX_HARD_MUTE 0x4001
// values
#define SI4735_MUTE 0x0003
#define SI4735_MUTE_LEFT 0x0002
#define SI4735_MUTE_RIGHT 0x0001




// ___ FM Properties __________________________________________________________________________________________________

/*
    FM DEEMPHASIS

    (Not applicable to Si4749)

    Sets the FM Receive de-emphasis to 50 or 75 μs. The CTS bit (and optional interrupt) is set when it is safe to send
    the next command. This property may only be set or read when in powerup mode. The default is 75 μs.

    Default: 0x0002
*/

// property
#define SI4735_FM_DEEMPHASIS 0x1100
// values
#define SI4735_EUR_50us 0x0001
#define SI4735_USA_75us 0x0002




/*
    FM CHANNEL FILTER

    (Si4706/49 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Selects bandwidth of channel filter applied at the demodulation stage. Default is automatic which means the device
    automatically selects proper channel filter. The CTS bit (and optional interrupt) is set when it is safe to send the
    next command. This property may only be set or read when in powerup mode. The default is 1.

    Default: 0x0001
    Range: 0–4

    Note: Automatic channel filter setting is not supported in FMRX component 3.0.
*/

// property
#define SI4735_FM_CHANNEL_FILTER 0x1102
// values
#define SI4735_AUTO 0x0000
#define SI4735_WIDE_110KHz 0x0001
#define SI4735_NARROW_84KHz 0x0002
#define SI4735_NARROWER_60KHz 0x0003
#define SI4735_NARROWEST_40KHz 0x0004




/*
    FM BLEND STEREO THRESHOLD

    (Not applicable to Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later)

    Sets RSSI threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this
    to 0. To force mono, set this to 127. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 49 dBμV.

    Default: 0x0031
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_BLEND_STEREO_THRESHOLD 0x1105




/*
    FM BLEND MONO THRESHOLD

    (Not applicable to Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later)

    Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set this
    to 0. To force mono, set this to 127. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 30 dBμV.

    Default: 0x001E
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_BLEND_MONO_THRESHOLD 0x1106




/*
    FM MAX TUNE ERROR

    Sets the maximum freq error allowed before setting the AFC rail indicator (AFCRL). The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 30 kHz.

    Note: For best seek performance set FM_MAX_TUNE_ERROR to 20 kHz.

    Default: 0x001E
    Units: kHz
    Step: 1
    Range: 0–255
*/

// property
#define SI4735_FM_MAX_TUNE_ERROR 0x1108




/*
    FM RSQ INT SOURCE

    Configures interrupt related to Received Signal Quality metrics. The CTS bit (and optional interrupt) is set when it is
    safe to send the next command. This property may only be set or read when in powerup mode. The default is 0.

    Default: 0x0000
*/

// property
#define SI4735_FM_RSQ_INT_SOURCE 0x1200
// values
// Enable blend as the source of interrupt. The threshold is set by SI4735_FM_RSQ_BLEND_THRESHOLD.
#define SI4735_BLENDIEN 0x0080
// Enable Multipath high as source of interrupt. Threshold is set by SI4735_FM_RSQ_MULTIPATH_HI_THRESHOLD.
#define SI4735_MULTHIEN 0x0020
// Enable Multipath low as source of interrupt. Threshold is set by SI4735_FM_RSQ_MULTIPATH_LO_THRESHOLD.
#define SI4735_MULTLIEN 0x0010
// Enable SNR high as the source of interrupt. The threshold is set by SI4735_FM_RSQ_SNR_HI_THRESHOLD.
#define SI4735_SNRHIEN  0x0008
// Enable SNR low as the source of interrupt. The threshold is set by SI4735_FM_RSQ_SNR_LO_THRESHOLD.
#define SI4735_SNRLIEN  0x0004
// Enable RSSI high as the source of interrupt. The threshold is set by SI4735_FM_RSQ_RSSI_HI_THRESHOLD.
#define SI4735_RSSIHIEN 0x0002
// Enable RSSI low as the source of interrupt. The threshold is set by SI4735_FM_RSQ_RSSI_LO_THRESHOLD.
#define SI4735_RSSILIEN 0x0001




/*
    FM RSQ SNR HI THRESHOLD

    Sets high threshold which triggers the RSQ interrupt if the SNR is above this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 127dB.

    Default: 0x007F
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_SNR_HI_THRESHOLD 0x1201




/*
    FM RSQ SNR LO THRESHOLD

    Sets low threshold which triggers the RSQ interrupt if the SNR is below this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 0 dB.

    Default: 0x0000
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_SNR_LO_THRESHOLD 0x1202




/*
    FM RSQ RSSI HI THRESHOLD

    Sets high threshold which triggers the RSQ interrupt if the RSSI is above this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 127 dBμV.

    Default: 0x007F
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_RSSI_HI_THRESHOLD 0x1203




/*
    FM RSQ RSSI LO THRESHOLD

    Sets low threshold which triggers the RSQ interrupt if the RSSI is below this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 0 dBμV.

    Default: 0x0000
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_RSSI_LO_THRESHOLD 0x1204




/*
    FM RSQ MULTIPATH HI THRESHOLD

    (Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the high threshold which triggers the RSQ interrupt if the Multipath level is above this threshold. The CTS bit
    (and optional interrupt) is set when it is safe to send the next command. This property may only be set or read
    when in power up mode. The default is 100.

    Default: 0x0064
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_MULTIPATH_HI_THRESHOLD 0x1205




/*
    FM RSQ MULTIPATH LO THRESHOLD

    (Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the low threshold which triggers the RSQ interrupt if the Multipath level is below this threshold. The CTS bit
    (and optional interrupt) is set when it is safe to send the next command. This property may only be set or read
    when in power up mode. The default is 0.

    Default: 0x0000
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_RSQ_MULTIPATH_LO_THRESHOLD 0x1206




/*
    FM RSQ BLEND THRESHOLD

    Sets the blend threshold for blend interrupt when boundary is crossed. The CTS bit (and optional interrupt) is set
    when it is safe to send the next command. This property may only be set or read when in powerup mode.
    The default is 1%.

    Default: 0x0081
    Units: %
    Step: 1
    Range: 0–100
*/

// property
#define SI4735_FM_RSQ_BLEND_THRESHOLD 0x1207
// values
#define SI4735_PILOT 0x0080




/*
    FM SOFT MUTE RATE

    (Not applicable to Si474x)

    Sets the attack and decay rates when entering and leaving soft mute. Higher values increase rates, and lower
    values decrease rates. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 64.

    Default: 64
    Step: 1
    Range: 1—255
*/

// property
#define SI4735_FM_SOFT_MUTE_RATE 0x1300




/*
    FM SOFT MUTE SLOPE 

    (Not Applicable to Si4749)

    Configures attenuation slope during soft mute in dB attenuation per dB SNR below the soft mute SNR threshold.
    Soft mute attenuation is the minimum of SMSLOPE x (SMTHR – SNR) and SMATTN. The recommended SMSLOPE value
    is CEILING(SMATTN/SMTHR). SMATTN and SMTHR are set via the FM_SOFT_MUTE_MAX_ATTENUATION and FM_SOFT_MUTE_SNR_THRESHOLD
    properties. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in power up mode.

    Default: 0x0002
    Range: 0–63
*/

// property
#define SI4735_FM_SOFT_MUTE_SLOPE  0x1301




/*
    FM SOFT MUTE MAX ATTENUATION

    (Not applicable to Si4749)

    Sets maximum attenuation during soft mute (dB). Set to 0 to disable soft mute. The CTS bit (and optional interrupt)
    is set when it is safe to send the next command. This property may only be set or read when in powerup mode. The
    default is 16 dB.

    Default: 0x0010
    Units: dB
    Step: 1
    Range: 0–31
*/

// property
#define SI4735_FM_SOFT_MUTE_MAX_ATTENUATION 0x1302




/*
    FM SOFT MUTE SNR THRESHOLD

    (Not applicable to Si4749)

    Sets SNR threshold to engage soft mute. Whenever the SNR for a tuned frequency drops below this threshold, the
    FM reception will go in soft mute, provided soft mute max attenuation property is non-zero. The CTS bit (and
    optional interrupt) is set when it is safe to send the next command. This property may only be set or read when in
    powerup mode. The default is 4 dB.

    Default: 0x0004
    Units: dB
    Step: 1
    Range: 0–15
*/

// property
#define SI4735_FM_SOFT_MUTE_SNR_THRESHOLD 0x1303




/*
    FM SOFT MUTE RELEASE RATE

    (Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the soft mute release rate. Smaller values provide slower release and larger values provide faster release.
    The CTS bit (and optional interrupt) is set when it is safe to send the next command. This property may only be set
    or read when in powerup mode. The default is 8192 (approximately 8000 dB/s).

    Release Rate (dB/s) = RELEASE[14:0]/1.024
    Default: 0x2000
    Range: 1–32767
*/

// property
#define SI4735_FM_SOFT_MUTE_RELEASE_RATE 0x1304




/*
    FM SOFT MUTE ATTACK RATE

    (Si4706-C30 and later, Si474x and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the soft mute attack rate. Smaller values provide slower attack and larger values provide faster attack. The
    CTS bit (and optional interrupt) is set when it is safe to send the next command. This property may only be set or
    read when in powerup mode. The default is 8192 (approximately 8000 dB/s).

    Attack Rate (dB/s) = ATTACK[14:0]/1.024
    Default: 0x2000
    Range: 1–32767
*/

// property
#define SI4735_FM_SOFT_MUTE_ATTACK_RATE 0x1305




/*
    FM SEEK BAND BOTTOM

    Sets the bottom of the FM band for seek. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 87.5 MHz.

    Default: 0x222E
    Units: 10 kHz
    Step: 50 kHz
    Range: 64–108 MHz
    Note: For FMRX components less than 2.0, range is 76–108 MHz.
*/

// property
#define SI4735_FM_SEEK_BAND_BOTTOM 0x1400




/*
    FM SEEK BAND TOP

    Sets the top of the FM band for seek. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 107.9 MHz.

    Default: 0x2A26
    Units: 10 kHz
    Step: 50 kHz
    Range: 64–108 MHz
    Note: For FMRX components less than 2.0, range is 76–108 MHz.
*/

// property
#define SI4735_FM_SEEK_BAND_TOP 0x1401




/*
    FM SEEK FREQ SPACING

    Selects frequency spacing for FM seek. There are only 3 valid values: 5, 10, and 20. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 100 kHz.

    Default: 0x000A
*/

// property
#define SI4735_FM_SEEK_FREQ_SPACING    0x1402
// values
#define SI4735_FM_SEEK_STEP_50KHz   0x0005
#define SI4735_FM_SEEK_STEP_100KHz  0x000a
#define SI4735_FM_SEEK_STEP_200KHz  0x0014




/*
    FM SEEK TUNE SNR THRESHOLD

    Sets the SNR threshold for a valid FM Seek/Tune. The CTS bit (and optional interrupt) is set when it is safe to send
    the next command. This property may only be set or read when in powerup mode. The default is 3 dB.

    Default: 0x0003
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_SEEK_TUNE_SNR_THRESHOLD 0x1403




/*
    FM SEEK TUNE RSSI THRESHOLD

    Sets the RSSI threshold for a valid FM Seek/Tune. The CTS bit (and optional interrupt) is set when it is safe to
    send the next command. This property may only be set or read when in powerup mode. The default is 20 dBμV.

    Default: 0x0014
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_SEEK_TUNE_RSSI_THRESHOLD 0x1404




/*
    RDS INT SOURCE

    (Si4705/06/21/31/35/37/39/41/43/45/49 only)

    Configures interrupt related to RDS. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 0.

    Note: RDS_INT_SOURCE is supported in FMRX comp 2.0 or higher.

    Default: 0x0000
*/

// property
#define SI4735_RDS_INT_SOURCE  0x1500
// values
#define SI4735_RDSNEWBLOCKBINT 0x0020  // If set, generate an interrupt when Block B data is found or subsequently changed.
#define SI4735_RDSNEWBLOCKAINT 0x0010  // If set, generate an interrupt when Block A data is found or subsequently changed.
#define SI4735_RDSSYNCFOUNDINT 0x0004  // If set, generate RDSINT when RDS gains synchronization.
#define SI4735_RDSSYNCLOSTINT  0x0002  // If set, generate RDSINT when RDS loses synchronization.
#define SI4735_RDSRECVINT      0x0001  // If set, generate RDSINT when RDS FIFO has at least RDS_INT_FIFO_COUNT entries.




/*
    RDS INT FIFO COUNT

    (Si4705/06/21/31/35/37/39/41/43/45/49 Only)

    Sets the minimum number of RDS groups stored in the RDS FIFO before RDSRECV is set. The maximum value is
    14. The CTS bit (and optional interrupt) is set when it is safe to send the next command. This property may only be
    set or read when in powerup mode. Default is 0.

    Note: RDS_INT_FIFO_COUNT is supported in FMRX comp 2.0 or higher.
    Default: 0x0000
    Range: 0–14
*/

// property
#define SI4735_RDS_INT_FIFO_COUNT 0x1501




/*
    RDS CONFIG

    (Si4705/06/21/31/35/37/39/41/43/45/49 only)

    Configures RDS settings to enable RDS processing (RDSEN) and set RDS block error thresholds. When a RDS
    Group is received, all block errors must be less than or equal the associated block error threshold for the group to
    be stored in the RDS FIFO. If blocks with errors are permitted into the FIFO, the block error information can be
    reviewed when the group is read using the FM_RDS_STATUS command. The CTS bit (and optional interrupt) is
    set when it is safe to send the next command. This property may only be set or read when in powerup mode. The
    default is 0x0000.

    Note: RDS_CONFIG is supported in FMRX comp 2.0 or higher.
    Default: 0x0000
*/

// property
#define SI4735_RDS_CONFIG           0x1502
// values
#define SI4735_BLETHA_NO_ERRORS     0x0000  // No errors in RDS block A.
#define SI4735_BLETHA_1_2_ERRORS    0x4000  // 1 to 2 corrected errors in RDS block A.
#define SI4735_BLETHA_3_5_ERRORS    0x8000  // 3 to 5 corrected errors in RDS block A.
#define SI4735_BLETHA_UNCORRECTABLE 0xc000  // Block A contains uncorrectable errors.

#define SI4735_BLETHB_NO_ERRORS     0x0000  // No errors in RDS block B.
#define SI4735_BLETHB_1_2_ERRORS    0x1000  // 1 to 2 corrected errors in RDS block B.
#define SI4735_BLETHB_3_5_ERRORS    0x2000  // 3 to 5 corrected errors in RDS block B.
#define SI4735_BLETHB_UNCORRECTABLE 0x3000  // Block B contains uncorrectable errors.

#define SI4735_BLETHC_NO_ERRORS     0x0000  // No errors in RDS block C.
#define SI4735_BLETHC_1_2_ERRORS    0x0400  // 1 to 2 corrected errors in RDS block C.
#define SI4735_BLETHC_3_5_ERRORS    0x0800  // 3 to 5 corrected errors in RDS block C.
#define SI4735_BLETHC_UNCORRECTABLE 0x0c00  // Block C contains uncorrectable errors.

#define SI4735_BLETHD_NO_ERRORS     0x0000  // No errors in RDS block D.
#define SI4735_BLETHD_1_2_ERRORS    0x0100  // 1 to 2 corrected errors in RDS block D.
#define SI4735_BLETHD_3_5_ERRORS    0x0200  // 3 to 5 corrected errors in RDS block D.
#define SI4735_BLETHD_UNCORRECTABLE 0x0300  // Block D contains uncorrectable errors.

#define SI4735_RDSEN                0x0001  // RDS processing enable.




/*
    RDS CONFIDENCE

    (Si4706, Si4741/43/45/49, and Si4705/31/35/85-D50 and later only)

    Selects the confidence level requirement for each RDS block. A higher confidence requirement will result in fewer
    decoder errors (% of blocks with BLE<3 that contains incorrect information) but more block errors (% of blocks with
    BLE=3). The CTS bit (and optional interrupt) is set when it is safe to send the next command. This property may
    only be set or read when in powerup mode.

    property bits 11-8: 4 bit confidence value for block B
    property bits  7-4: 4 bit confidence value for block C
    property bits  3-0: 4 bit confidence value for block D

    Default: 0x1111
*/

// property
#define SI4735_RDS_CONFIDENCE 0x1503




/*
    FM BLEND SNR STEREO THRESHOLD 

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets SNR threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this
    to 0. To force mono, set this to 127. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 27 dB.

    Default: 0x001B
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_BLEND_SNR_STEREO_THRESHOLD 0x1804




/*
    FM BLEND SNR MONO THRESHOLD 

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets SNR threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set to 0.
    To force mono, set to 127. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 14 dB.

    Default: 0x000E
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_FM_BLEND_SNR_MONO_THRESHOLD 0x1805




/*
    FM BLEND SNR ATTACK RATE 

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the stereo to mono attack rate for SNR based blend. Smaller values provide slower attack and larger values
    provide faster attack. The CTS bit (and optional interrupt) is set when it is safe to send the next command. This
    property may only be set or read when in powerup mode. The default is 4000 (approximately 16 ms).

    ATTACK[15:0] = 65536/time, where time is the desired transition time in ms.

    Default: 0x0FA0
    Step: 1
    Range: 0 (disabled), 1–32767
*/

// property
#define SI4735_FM_BLEND_SNR_ATTACK_RATE 0x1806




/*
    FM BLEND SNR RELEASE RATE 

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the mono to stereo release rate for SNR based blend. Smaller values provide slower release and larger
    values provide faster release. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 400 (approximately 164 ms).

    RELEASE[15:0] = 65536/time, where time is the desired transition time in ms.

    Default: 0x0190
    Step: 1
    Range: 0 (disabled), 1–32767
*/

// property
#define SI4735_FM_BLEND_SNR_RELEASE_RATE 0x1807




/*
    FM BLEND MULTIPATH STEREO THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets Multipath threshold for stereo blend (Full stereo below threshold, blend above threshold). To force stereo, set
    to 100. To force mono, set to 0. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 20.

    Default: 0x0014
    Step: 1
    Range: 0–100
*/

// property
#define SI4735_FM_BLEND_MULTIPATH_STEREO_THRESHOLD 0x1808




/*
    FM BLEND MULTIPATH MONO THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets Multipath threshold for mono blend (Full mono above threshold, blend below threshold). To force stereo, set to
    100. To force mono, set to 0. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 60.

    Default: 0x003C
    Step: 1
    Range: 0–100
*/

// property
#define SI4735_FM_BLEND_MULTIPATH_MONO_THRESHOLD 0x1809




/*
    FM BLEND MULTIPATH ATTACK RATE

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the stereo to mono attack rate for Multipath based blend. Smaller values provide slower attack and larger
    values provide faster attack. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 4000 (approximately 16 ms).

    ATTACK[15:0] = 65536/time, where time is the desired transition time in ms.

    Default: 0x0FA0
    Step: 1
    Range: 0 (disabled), 1–32767
*/

// property
#define SI4735_FM_BLEND_MULTIPATH_ATTACK_RATE 0x180a




/*
    FM BLEND MULTIPATH RELEASE RATE

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the mono to stereo release rate for Multipath based blend. Smaller values provide slower release and larger
    values provide faster release. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 40 (approximately 1.64 s).

    RELEASE[15:0] = 65536/time, where time is the desired transition time in ms.

    Default: 0x0028
    Step: 1
    Range: 0 (disabled), 1–32767
*/

// property
#define SI4735_FM_BLEND_MULTIPATH_RELEASE_RATE 0x180b




/*
    FM HICUT SNR HIGH THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the SNR level at which hi-cut begins to band limit. The CTS bit (and optional interrupt) is set when it is safe to
    send the next command. This property may only be set or read in POWERUP mode. The default is 24 dB.

    Default: 0x0018
    Range: 0–127
    Note: Was property 0x180C in FW2.B.
*/

// property
#define SI4735_FM_HICUT_SNR_HIGH_THRESHOLD 0x1a00




/*
    FM HICUT SNR LOW THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the SNR level at which hi-cut reaches maximum band limiting. The CTS bit (and optional interrupt) is set when
    it is safe to send the next command. This property may only be set or read in POWERUP mode. The default is 15 dB.

    Default: 0x000F
    Range: 0–127
    Note: Was property 0x180D in FW2.B.
*/

// property
#define SI4735_FM_HICUT_SNR_LOW_THRESHOLD 0x1a01




/*
    FM HICUT ATTACK RATE

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the rate at which hi-cut lowers the transition frequency. The CTS bit (and optional interrupt) is set when it is
    safe to send the next command. This property may only be set or read in POWERUP mode. The default is 20000
    (approximately 3 ms).

    ATTACK[15:0] = 65536/time, were time is the desired transition time in ms.

    Default: 0x4E20
    Range: 0 (disabled), 1–32767
    Note: Was property 0x180E in FW2.B.
*/

// property
#define SI4735_FM_HICUT_ATTACK_RATE 0x1a02




/*
    FM HICUT RELEASE RATE

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the rate at which hi-cut increases the transition frequency. The CTS bit (and optional interrupt) is set when it is
    safe to send the next command. This property may only be set or read in POWERUP mode. The default is 20
    (approximately 3.3 s).

    RELEASE[15:0] = 65536/time, were time is the desired transition time in ms.

    Default: 0x0014
    Range: 0 (disabled), 1–32767
    Note: Was property 0x180F in FW2.B.
*/

// property
#define SI4735_FM_HICUT_RELEASE_RATE 0x1a03




/*
    FM HICUT MULTIPATH TRIGGER THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the MULTIPATH level at which hi-cut begins to band limit. The CTS bit (and optional interrupt) is set when it is
    safe to send the next command. This property may only be set or read in POWERUP mode. The default is 20%.

    Default: 0x0014
    Range: 0–100
    Note: Was property 0x1810 in FW2.B.
*/

// property
#define SI4735_FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD 0x1a04




/*
    FM HICUT MULTIPATH END THRESHOLD

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the MULTIPATH level at which hi-cut reaches maximum band limiting. The CTS bit (and optional interrupt) is
    set when it is safe to send the next command. This property may only be set or read in POWERUP mode. The default is 60%.

    Default: 0x003C
    Range: 0–100
    Note: Was property 0x1811 in FW2.B.
*/

// property
#define SI4735_FM_HICUT_MULTIPATH_END_THRESHOLD 0x1a05




/*
    FM HICUT CUTOFF FREQUENCY

    (Si4706-C30 and later; Si4740/41/42/43/44/45 and Si4704/05/30/31/34/35/84/85-D50 and later only)

    Sets the maximum band limit frequency for hi-cut and also sets the maximum audio frequency. The CTS bit
    (optional interrupt) is set when it is safe to send the next command. This property may only be set or read in
    POWERUP mode. The default is 0(disabled).

    Default 0x0000
    Range: 0–7 (maximum band limit frequency for Hi-Cut)
    0–7 (maximum audio frequency)

    Note: Was property 0x1812 in FW2.B. The maximum audio frequency was not programmable in FW2.B.
*/

// property
#define SI4735_FM_HICUT_CUTOFF_FREQUENCY 0x1a06
// values
#define SI4735_MAX_AUDIO_TRANS_FREQ_FULL 0x0000
#define SI4735_MAX_AUDIO_TRANS_FREQ_2KHz 0x0001
#define SI4735_MAX_AUDIO_TRANS_FREQ_3KHz 0x0002
#define SI4735_MAX_AUDIO_TRANS_FREQ_4KHz 0x0003
#define SI4735_MAX_AUDIO_TRANS_FREQ_5KHz 0x0004
#define SI4735_MAX_AUDIO_TRANS_FREQ_6KHz 0x0005
#define SI4735_MAX_AUDIO_TRANS_FREQ_8KHz 0x0006
#define SI4735_MAX_AUDIO_TRANS_FREQ_11KHz 0x0007
#define SI4735_HICUT_DISABLED 0x0000
#define SI4735_HICUT_TRANS_FREQ_2KHz 0x0001
#define SI4735_HICUT_TRANS_FREQ_3KHz 0x0002
#define SI4735_HICUT_TRANS_FREQ_4KHz 0x0003
#define SI4735_HICUT_TRANS_FREQ_5KHz 0x0004
#define SI4735_HICUT_TRANS_FREQ_6KHz 0x0005
#define SI4735_HICUT_TRANS_FREQ_8KHz 0x0006
#define SI4735_HICUT_TRANS_FREQ_11KHz 0x0007




// ___ AM Properties ______________________________________________________________________________________________

/*
    AM DEEMPHASIS

    Sets the AM Receive de-emphasis to 50 μs. The CTS bit (and optional interrupt) is set when it is safe to send the
    next command. This property may only be set or read when in powerup mode. The default is disabled.

    Default: 0x0000
*/

// property
#define SI4735_AM_DEEMPHASIS 0x3100
// values
#define SI4735_AM_50us       0x0001
#define SI4735_DISABLE       0x0000




/*
     AM CHANNEL FILTER

     Selects the bandwidth of the AM channel filter. The CTS bit (and optional interrupt) is set when it is safe to send the
     next command. This property may only be set or read when in powerup mode. The default is 2 kHz bandwidth
     channel filter.

     Note:
     The 1 kHz option is supported on AM_SW_LW component 3.0 or later.
     The 1.8 kHz option is supported on AM_SW_LW component 3.0 or later.
     The 2.5 kHz option is supported on AM_SW_LW component 5.0 or later.

     Default: 0x0003
*/

// property
#define SI4735_AM_CHANNEL_FILTER 0x3102
// values
#define SI4735_AMPLFLT             0x0100  // Enables the power line rejection filter.

#define SI4735_BW6KHZ              0x0000  // Channel bandwidth filter values.
#define SI4735_BW4KHZ              0x0001
#define SI4735_BW3KHZ              0x0002
#define SI4735_BW2KHZ              0x0003
#define SI4735_BW1KHZ              0x0004
#define SI4735_BW1800HZ            0x0005
#define SI4735_BW2500HZ            0x0006




/*
    AM AUTOMATIC VOLUME CONTROL MAX GAIN

    Sets the maximum gain for automatic volume control. The CTS bit (and optional interrupt) is set when it is safe to
    send the next command. This property may only be set or read when in powerup mode. The default is 16 dB.
    Note: For AMRX components 3.0 or earlier, the default is set to 90.3 dB.
    AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN property is supported in AMRX component 3.0 or later.

    Default: 0x1543
    Step: 1
    Range: 0X1000 ~ 0x7800
*/

// property
#define SI4735_AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN 0x3203




/*
    AM RSQ INT SOURCE

    Configures interrupt related to Received Signal Quality metrics. The CTS bit (and optional interrupt) is set
    when it is safe to send the next command. This property may only be set or read when in powerup mode.

    Default: 0x0000
*/

// property
#define SI4735_AM_RSQ_INT_SOURCE 0x3200
/*
   the values have been defined in FM_RSQ_INT_SOURCE

   Enable SNR high as the source of interrupt. The threshold is set by SI4735_AM_RSQ_SNR_HI_THRESHOLD.
   SI4735_SNRHIEN  0x0008
   Enable SNR low as the as the source of interrupt. The threshold is set by SI4735_AM_RSQ_SNR_LO_THRESHOLD.
   SI4735_SNRLIEN  0x0004
   Enable RSSI high as the source of interrupt. The threshold is set by SI4735_AM_RSQ_RSSI_HI_THRESHOLD.
   SI4735_RSSIHIEN 0x0002
   Enable RSSI low as the source of interrupt. The threshold is set by SI4735_AM_RSQ_RSSI_LO_THRESHOLD.
   SI4735_RSSILIEN 0x0001
*/




/*
    AM RSQ SNR HI THRESHOLD

    Sets high threshold which triggers the RSQ interrupt if the SNR is above this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 127 dB.

    Default: 0x007F
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_AM_RSQ_SNR_HI_THRESHOLD 0x3201




/*
    AM RSQ SNR LO THRESHOLD

    Sets low threshold which triggers the RSQ interrupt if the SNR is below this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 0 dB.

    Default: 0x0000
    Units: dB
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_AM_RSQ_SNR_LO_THRESHOLD 0x3202




/*
    AM RSQ RSSI HI THRESHOLD

    Sets high threshold which triggers the RSQ interrupt if the RSSI is above this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 127 dB.

    Default: 0x007F
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_AM_RSQ_RSSI_HI_THRESHOLD 0x3203




/*
    AM RSQ RSSI LO THRESHOLD

    Sets low threshold which triggers the RSQ interrupt if the RSSI is below this threshold. The CTS bit (and optional
    interrupt) is set when it is safe to send the next command. This property may only be set or read when in powerup
    mode. The default is 0 dB.

    Default: 0x0000
    Units: dBμV
    Step: 1
    Range: 0–127
*/

// property
#define SI4735_AM_RSQ_RSSI_LO_THRESHOLD 0x3204




/*
    AM SOFT MUTE RATE

    Sets the attack and decay rates when entering or leaving soft mute. The value specified is multiplied by 4.35 dB/s
    to come up with the actual attack rate. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default rate is 278 dB/s.

    Default: 0x0040
    Actual Rate: SMRATE x 4.35
    Units: dB/s
    Step: 1
    Range: 1–255
*/

// property
#define SI4735_AM_SOFT_MUTE_RATE 0x3300




/*
    AM SOFT MUTE ATTACK RATE

    Sets the soft mute attack rate. Smaller values provide slower attack and larger values provide faster attack. The
    CTS bit (and optional interrupt) is set when it is safe to send the next command. This property may only be set or
    read when in powerup mode. The default is 8192 (approximately 8000 dB/s).

    Attack Rate (dB/s) = ATTACK[14:0]/1.024

    Default: 0x2000
    Range: 1–32767
*/

// property
#define SI4735_AM_SOFT_MUTE_ATTACK_RATE 0x3305




/*
    AM SEEK BAND BOTTOM

    Sets the lower boundary for the AM band in kHz. This value is used to determine when the lower end of the AM
    band is reached when performing a seek. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 520 kHz (0x0208).

    Default: 0x0208
    Units: kHz
    Step: 1 kHz
    Valid Range: 149–23000 kHz
*/

// property
#define SI4735_AM_SEEK_BAND_BOTTOM 0x3400




/*
    AM SEEK BAND TOP

    Sets the upper boundary for the AM band in kHz. This value is used to determine when the higher end of the AM
    band is reached when performing a seek. The CTS bit (and optional interrupt) is set when it is safe to send the next
    command. This property may only be set or read when in powerup mode. The default is 1710 kHz (0x06AE).

    Default: 0x06AE
    Note: Firmware 1.0 incorrectly reports 0x06B9 (1721 kHz) as default for AM_SEEK_BAND_TOP. After POWER_UP
    command is complete, set AM_SEEK_BAND_TOP to 0x06AE (1710 kHz) using the SET_PROPERTY command.
    Units: kHz
    Step: 1 kHz
    Valid Range: 149–23000 kHz
*/

// property
#define SI4735_AM_SEEK_BAND_TOP 0x3401




/*
    AM SEEK FREQ SPACING

    Sets the frequency spacing for the AM Band when performing a seek. The frequency spacing determines how far
    the next tune is going to be from the currently tuned frequency. The CTS bit (and optional interrupt) is set when it is
    safe to send the next command. This property may only be set or read when in powerup mode. The default
    frequency spacing is 10 kHz.

    Default: 0x000A
    Units: kHz
    Valid Values: 1 (1 kHz), 5 (5 kHz), 9 (9 kHz), and 10 (10 kHz).
*/

// property
#define SI4735_AM_SEEK_FREQ_SPACING  0x3402
// values
#define SI4735_AM_SEEK_STEP_1KHZ  0x0001
#define SI4735_AM_SEEK_STEP_5KHZ  0x0005
#define SI4735_AM_SEEK_STEP_9KHZ  0x0009
#define SI4735_AM_SEEK_STEP_10KHZ 0x000a




/*
    AM_SEEK_TUNE_SNR_THRESHOLD

    Sets the SNR threshold for a valid AM Seek/Tune. If the value is zero, then SNR is not used as a valid criteria when
    doing a seek for AM. The CTS bit (and optional interrupt) is set when it is safe to send the next command. This
    property may only be set or read when in powerup mode. The default threshold is 5 dB.

    Default: 0x0005
    Units: dB
    Step: 1
    Range: 0–63
*/

// property
#define SI4735_AM_SEEK_TUNE_SNR_THRESHOLD 0x3403




/*
    AM SEEK TUNE RSSI THRESHOLD

    Sets the RSSI threshold for a valid AM Seek/Tune. If the value is zero then RSSI is not used as a valid criteria
    when doing a seek for AM. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    This property may only be set or read when in powerup mode. The default is 25 dBμV.

    Default: 0x0019
    Units: dBμV
    Step: 1
    Range: 0–63
*/

// property
#define SI4735_AM_SEEK_TUNE_RSSI_THRESHOLD 0x3404



#endif
