EXTERN g_winmm_procs:QWORD
.code
 proxy_CloseDriver PROC
    jmp QWORD PTR [g_winmm_procs + 0]
 proxy_CloseDriver ENDP
 proxy_DefDriverProc PROC
    jmp QWORD PTR [g_winmm_procs + 8]
 proxy_DefDriverProc ENDP
 proxy_DriverCallback PROC
    jmp QWORD PTR [g_winmm_procs + 16]
 proxy_DriverCallback ENDP
 proxy_DrvGetModuleHandle PROC
    jmp QWORD PTR [g_winmm_procs + 24]
 proxy_DrvGetModuleHandle ENDP
 proxy_GetDriverModuleHandle PROC
    jmp QWORD PTR [g_winmm_procs + 32]
 proxy_GetDriverModuleHandle ENDP
 proxy_OpenDriver PROC
    jmp QWORD PTR [g_winmm_procs + 40]
 proxy_OpenDriver ENDP
 proxy_PlaySound PROC
    jmp QWORD PTR [g_winmm_procs + 48]
 proxy_PlaySound ENDP
 proxy_PlaySoundA PROC
    jmp QWORD PTR [g_winmm_procs + 56]
 proxy_PlaySoundA ENDP
 proxy_PlaySoundW PROC
    jmp QWORD PTR [g_winmm_procs + 64]
 proxy_PlaySoundW ENDP
 proxy_SendDriverMessage PROC
    jmp QWORD PTR [g_winmm_procs + 72]
 proxy_SendDriverMessage ENDP
 proxy_WOWAppExit PROC
    jmp QWORD PTR [g_winmm_procs + 80]
 proxy_WOWAppExit ENDP
 proxy_auxGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 88]
 proxy_auxGetDevCapsA ENDP
 proxy_auxGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 96]
 proxy_auxGetDevCapsW ENDP
 proxy_auxGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 104]
 proxy_auxGetNumDevs ENDP
 proxy_auxGetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 112]
 proxy_auxGetVolume ENDP
 proxy_auxOutMessage PROC
    jmp QWORD PTR [g_winmm_procs + 120]
 proxy_auxOutMessage ENDP
 proxy_auxSetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 128]
 proxy_auxSetVolume ENDP
 proxy_joyConfigChanged PROC
    jmp QWORD PTR [g_winmm_procs + 136]
 proxy_joyConfigChanged ENDP
 proxy_joyGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 144]
 proxy_joyGetDevCapsA ENDP
 proxy_joyGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 152]
 proxy_joyGetDevCapsW ENDP
 proxy_joyGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 160]
 proxy_joyGetNumDevs ENDP
 proxy_joyGetPos PROC
    jmp QWORD PTR [g_winmm_procs + 168]
 proxy_joyGetPos ENDP
 proxy_joyGetPosEx PROC
    jmp QWORD PTR [g_winmm_procs + 176]
 proxy_joyGetPosEx ENDP
 proxy_joyGetThreshold PROC
    jmp QWORD PTR [g_winmm_procs + 184]
 proxy_joyGetThreshold ENDP
 proxy_joyReleaseCapture PROC
    jmp QWORD PTR [g_winmm_procs + 192]
 proxy_joyReleaseCapture ENDP
 proxy_joySetCapture PROC
    jmp QWORD PTR [g_winmm_procs + 200]
 proxy_joySetCapture ENDP
 proxy_joySetThreshold PROC
    jmp QWORD PTR [g_winmm_procs + 208]
 proxy_joySetThreshold ENDP
 proxy_mciDriverNotify PROC
    jmp QWORD PTR [g_winmm_procs + 216]
 proxy_mciDriverNotify ENDP
 proxy_mciDriverYield PROC
    jmp QWORD PTR [g_winmm_procs + 224]
 proxy_mciDriverYield ENDP
 proxy_mciExecute PROC
    jmp QWORD PTR [g_winmm_procs + 232]
 proxy_mciExecute ENDP
 proxy_mciFreeCommandResource PROC
    jmp QWORD PTR [g_winmm_procs + 240]
 proxy_mciFreeCommandResource ENDP
 proxy_mciGetCreatorTask PROC
    jmp QWORD PTR [g_winmm_procs + 248]
 proxy_mciGetCreatorTask ENDP
 proxy_mciGetDeviceIDA PROC
    jmp QWORD PTR [g_winmm_procs + 256]
 proxy_mciGetDeviceIDA ENDP
 proxy_mciGetDeviceIDFromElementIDA PROC
    jmp QWORD PTR [g_winmm_procs + 264]
 proxy_mciGetDeviceIDFromElementIDA ENDP
 proxy_mciGetDeviceIDFromElementIDW PROC
    jmp QWORD PTR [g_winmm_procs + 272]
 proxy_mciGetDeviceIDFromElementIDW ENDP
 proxy_mciGetDeviceIDW PROC
    jmp QWORD PTR [g_winmm_procs + 280]
 proxy_mciGetDeviceIDW ENDP
 proxy_mciGetDriverData PROC
    jmp QWORD PTR [g_winmm_procs + 288]
 proxy_mciGetDriverData ENDP
 proxy_mciGetErrorStringA PROC
    jmp QWORD PTR [g_winmm_procs + 296]
 proxy_mciGetErrorStringA ENDP
 proxy_mciGetErrorStringW PROC
    jmp QWORD PTR [g_winmm_procs + 304]
 proxy_mciGetErrorStringW ENDP
 proxy_mciGetYieldProc PROC
    jmp QWORD PTR [g_winmm_procs + 312]
 proxy_mciGetYieldProc ENDP
 proxy_mciLoadCommandResource PROC
    jmp QWORD PTR [g_winmm_procs + 320]
 proxy_mciLoadCommandResource ENDP
 proxy_mciSendCommandA PROC
    jmp QWORD PTR [g_winmm_procs + 328]
 proxy_mciSendCommandA ENDP
 proxy_mciSendCommandW PROC
    jmp QWORD PTR [g_winmm_procs + 336]
 proxy_mciSendCommandW ENDP
 proxy_mciSendStringA PROC
    jmp QWORD PTR [g_winmm_procs + 344]
 proxy_mciSendStringA ENDP
 proxy_mciSendStringW PROC
    jmp QWORD PTR [g_winmm_procs + 352]
 proxy_mciSendStringW ENDP
 proxy_mciSetDriverData PROC
    jmp QWORD PTR [g_winmm_procs + 360]
 proxy_mciSetDriverData ENDP
 proxy_mciSetYieldProc PROC
    jmp QWORD PTR [g_winmm_procs + 368]
 proxy_mciSetYieldProc ENDP
 proxy_midiConnect PROC
    jmp QWORD PTR [g_winmm_procs + 376]
 proxy_midiConnect ENDP
 proxy_midiDisconnect PROC
    jmp QWORD PTR [g_winmm_procs + 384]
 proxy_midiDisconnect ENDP
 proxy_midiInAddBuffer PROC
    jmp QWORD PTR [g_winmm_procs + 392]
 proxy_midiInAddBuffer ENDP
 proxy_midiInClose PROC
    jmp QWORD PTR [g_winmm_procs + 400]
 proxy_midiInClose ENDP
 proxy_midiInGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 408]
 proxy_midiInGetDevCapsA ENDP
 proxy_midiInGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 416]
 proxy_midiInGetDevCapsW ENDP
 proxy_midiInGetErrorTextA PROC
    jmp QWORD PTR [g_winmm_procs + 424]
 proxy_midiInGetErrorTextA ENDP
 proxy_midiInGetErrorTextW PROC
    jmp QWORD PTR [g_winmm_procs + 432]
 proxy_midiInGetErrorTextW ENDP
 proxy_midiInGetID PROC
    jmp QWORD PTR [g_winmm_procs + 440]
 proxy_midiInGetID ENDP
 proxy_midiInGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 448]
 proxy_midiInGetNumDevs ENDP
 proxy_midiInMessage PROC
    jmp QWORD PTR [g_winmm_procs + 456]
 proxy_midiInMessage ENDP
 proxy_midiInOpen PROC
    jmp QWORD PTR [g_winmm_procs + 464]
 proxy_midiInOpen ENDP
 proxy_midiInPrepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 472]
 proxy_midiInPrepareHeader ENDP
 proxy_midiInReset PROC
    jmp QWORD PTR [g_winmm_procs + 480]
 proxy_midiInReset ENDP
 proxy_midiInStart PROC
    jmp QWORD PTR [g_winmm_procs + 488]
 proxy_midiInStart ENDP
 proxy_midiInStop PROC
    jmp QWORD PTR [g_winmm_procs + 496]
 proxy_midiInStop ENDP
 proxy_midiInUnprepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 504]
 proxy_midiInUnprepareHeader ENDP
 proxy_midiOutCacheDrumPatches PROC
    jmp QWORD PTR [g_winmm_procs + 512]
 proxy_midiOutCacheDrumPatches ENDP
 proxy_midiOutCachePatches PROC
    jmp QWORD PTR [g_winmm_procs + 520]
 proxy_midiOutCachePatches ENDP
 proxy_midiOutClose PROC
    jmp QWORD PTR [g_winmm_procs + 528]
 proxy_midiOutClose ENDP
 proxy_midiOutGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 536]
 proxy_midiOutGetDevCapsA ENDP
 proxy_midiOutGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 544]
 proxy_midiOutGetDevCapsW ENDP
 proxy_midiOutGetErrorTextA PROC
    jmp QWORD PTR [g_winmm_procs + 552]
 proxy_midiOutGetErrorTextA ENDP
 proxy_midiOutGetErrorTextW PROC
    jmp QWORD PTR [g_winmm_procs + 560]
 proxy_midiOutGetErrorTextW ENDP
 proxy_midiOutGetID PROC
    jmp QWORD PTR [g_winmm_procs + 568]
 proxy_midiOutGetID ENDP
 proxy_midiOutGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 576]
 proxy_midiOutGetNumDevs ENDP
 proxy_midiOutGetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 584]
 proxy_midiOutGetVolume ENDP
 proxy_midiOutLongMsg PROC
    jmp QWORD PTR [g_winmm_procs + 592]
 proxy_midiOutLongMsg ENDP
 proxy_midiOutMessage PROC
    jmp QWORD PTR [g_winmm_procs + 600]
 proxy_midiOutMessage ENDP
 proxy_midiOutOpen PROC
    jmp QWORD PTR [g_winmm_procs + 608]
 proxy_midiOutOpen ENDP
 proxy_midiOutPrepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 616]
 proxy_midiOutPrepareHeader ENDP
 proxy_midiOutReset PROC
    jmp QWORD PTR [g_winmm_procs + 624]
 proxy_midiOutReset ENDP
 proxy_midiOutSetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 632]
 proxy_midiOutSetVolume ENDP
 proxy_midiOutShortMsg PROC
    jmp QWORD PTR [g_winmm_procs + 640]
 proxy_midiOutShortMsg ENDP
 proxy_midiOutUnprepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 648]
 proxy_midiOutUnprepareHeader ENDP
 proxy_midiStreamClose PROC
    jmp QWORD PTR [g_winmm_procs + 656]
 proxy_midiStreamClose ENDP
 proxy_midiStreamOpen PROC
    jmp QWORD PTR [g_winmm_procs + 664]
 proxy_midiStreamOpen ENDP
 proxy_midiStreamOut PROC
    jmp QWORD PTR [g_winmm_procs + 672]
 proxy_midiStreamOut ENDP
 proxy_midiStreamPause PROC
    jmp QWORD PTR [g_winmm_procs + 680]
 proxy_midiStreamPause ENDP
 proxy_midiStreamPosition PROC
    jmp QWORD PTR [g_winmm_procs + 688]
 proxy_midiStreamPosition ENDP
 proxy_midiStreamProperty PROC
    jmp QWORD PTR [g_winmm_procs + 696]
 proxy_midiStreamProperty ENDP
 proxy_midiStreamRestart PROC
    jmp QWORD PTR [g_winmm_procs + 704]
 proxy_midiStreamRestart ENDP
 proxy_midiStreamStop PROC
    jmp QWORD PTR [g_winmm_procs + 712]
 proxy_midiStreamStop ENDP
 proxy_mixerClose PROC
    jmp QWORD PTR [g_winmm_procs + 720]
 proxy_mixerClose ENDP
 proxy_mixerGetControlDetailsA PROC
    jmp QWORD PTR [g_winmm_procs + 728]
 proxy_mixerGetControlDetailsA ENDP
 proxy_mixerGetControlDetailsW PROC
    jmp QWORD PTR [g_winmm_procs + 736]
 proxy_mixerGetControlDetailsW ENDP
 proxy_mixerGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 744]
 proxy_mixerGetDevCapsA ENDP
 proxy_mixerGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 752]
 proxy_mixerGetDevCapsW ENDP
 proxy_mixerGetID PROC
    jmp QWORD PTR [g_winmm_procs + 760]
 proxy_mixerGetID ENDP
 proxy_mixerGetLineControlsA PROC
    jmp QWORD PTR [g_winmm_procs + 768]
 proxy_mixerGetLineControlsA ENDP
 proxy_mixerGetLineControlsW PROC
    jmp QWORD PTR [g_winmm_procs + 776]
 proxy_mixerGetLineControlsW ENDP
 proxy_mixerGetLineInfoA PROC
    jmp QWORD PTR [g_winmm_procs + 784]
 proxy_mixerGetLineInfoA ENDP
 proxy_mixerGetLineInfoW PROC
    jmp QWORD PTR [g_winmm_procs + 792]
 proxy_mixerGetLineInfoW ENDP
 proxy_mixerGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 800]
 proxy_mixerGetNumDevs ENDP
 proxy_mixerMessage PROC
    jmp QWORD PTR [g_winmm_procs + 808]
 proxy_mixerMessage ENDP
 proxy_mixerOpen PROC
    jmp QWORD PTR [g_winmm_procs + 816]
 proxy_mixerOpen ENDP
 proxy_mixerSetControlDetails PROC
    jmp QWORD PTR [g_winmm_procs + 824]
 proxy_mixerSetControlDetails ENDP
 proxy_mmDrvInstall PROC
    jmp QWORD PTR [g_winmm_procs + 832]
 proxy_mmDrvInstall ENDP
 proxy_mmGetCurrentTask PROC
    jmp QWORD PTR [g_winmm_procs + 840]
 proxy_mmGetCurrentTask ENDP
 proxy_mmTaskBlock PROC
    jmp QWORD PTR [g_winmm_procs + 848]
 proxy_mmTaskBlock ENDP
 proxy_mmTaskCreate PROC
    jmp QWORD PTR [g_winmm_procs + 856]
 proxy_mmTaskCreate ENDP
 proxy_mmTaskSignal PROC
    jmp QWORD PTR [g_winmm_procs + 864]
 proxy_mmTaskSignal ENDP
 proxy_mmTaskYield PROC
    jmp QWORD PTR [g_winmm_procs + 872]
 proxy_mmTaskYield ENDP
 proxy_mmioAdvance PROC
    jmp QWORD PTR [g_winmm_procs + 880]
 proxy_mmioAdvance ENDP
 proxy_mmioAscend PROC
    jmp QWORD PTR [g_winmm_procs + 888]
 proxy_mmioAscend ENDP
 proxy_mmioClose PROC
    jmp QWORD PTR [g_winmm_procs + 896]
 proxy_mmioClose ENDP
 proxy_mmioCreateChunk PROC
    jmp QWORD PTR [g_winmm_procs + 904]
 proxy_mmioCreateChunk ENDP
 proxy_mmioDescend PROC
    jmp QWORD PTR [g_winmm_procs + 912]
 proxy_mmioDescend ENDP
 proxy_mmioFlush PROC
    jmp QWORD PTR [g_winmm_procs + 920]
 proxy_mmioFlush ENDP
 proxy_mmioGetInfo PROC
    jmp QWORD PTR [g_winmm_procs + 928]
 proxy_mmioGetInfo ENDP
 proxy_mmioInstallIOProcA PROC
    jmp QWORD PTR [g_winmm_procs + 936]
 proxy_mmioInstallIOProcA ENDP
 proxy_mmioInstallIOProcW PROC
    jmp QWORD PTR [g_winmm_procs + 944]
 proxy_mmioInstallIOProcW ENDP
 proxy_mmioOpenA PROC
    jmp QWORD PTR [g_winmm_procs + 952]
 proxy_mmioOpenA ENDP
 proxy_mmioOpenW PROC
    jmp QWORD PTR [g_winmm_procs + 960]
 proxy_mmioOpenW ENDP
 proxy_mmioRead PROC
    jmp QWORD PTR [g_winmm_procs + 968]
 proxy_mmioRead ENDP
 proxy_mmioRenameA PROC
    jmp QWORD PTR [g_winmm_procs + 976]
 proxy_mmioRenameA ENDP
 proxy_mmioRenameW PROC
    jmp QWORD PTR [g_winmm_procs + 984]
 proxy_mmioRenameW ENDP
 proxy_mmioSeek PROC
    jmp QWORD PTR [g_winmm_procs + 992]
 proxy_mmioSeek ENDP
 proxy_mmioSendMessage PROC
    jmp QWORD PTR [g_winmm_procs + 1000]
 proxy_mmioSendMessage ENDP
 proxy_mmioSetBuffer PROC
    jmp QWORD PTR [g_winmm_procs + 1008]
 proxy_mmioSetBuffer ENDP
 proxy_mmioSetInfo PROC
    jmp QWORD PTR [g_winmm_procs + 1016]
 proxy_mmioSetInfo ENDP
 proxy_mmioStringToFOURCCA PROC
    jmp QWORD PTR [g_winmm_procs + 1024]
 proxy_mmioStringToFOURCCA ENDP
 proxy_mmioStringToFOURCCW PROC
    jmp QWORD PTR [g_winmm_procs + 1032]
 proxy_mmioStringToFOURCCW ENDP
 proxy_mmioWrite PROC
    jmp QWORD PTR [g_winmm_procs + 1040]
 proxy_mmioWrite ENDP
 proxy_mmsystemGetVersion PROC
    jmp QWORD PTR [g_winmm_procs + 1048]
 proxy_mmsystemGetVersion ENDP
 proxy_sndPlaySoundA PROC
    jmp QWORD PTR [g_winmm_procs + 1056]
 proxy_sndPlaySoundA ENDP
 proxy_sndPlaySoundW PROC
    jmp QWORD PTR [g_winmm_procs + 1064]
 proxy_sndPlaySoundW ENDP
 proxy_timeBeginPeriod PROC
    jmp QWORD PTR [g_winmm_procs + 1072]
 proxy_timeBeginPeriod ENDP
 proxy_timeEndPeriod PROC
    jmp QWORD PTR [g_winmm_procs + 1080]
 proxy_timeEndPeriod ENDP
 proxy_timeGetDevCaps PROC
    jmp QWORD PTR [g_winmm_procs + 1088]
 proxy_timeGetDevCaps ENDP
 proxy_timeGetSystemTime PROC
    jmp QWORD PTR [g_winmm_procs + 1096]
 proxy_timeGetSystemTime ENDP
 proxy_timeGetTime PROC
    jmp QWORD PTR [g_winmm_procs + 1104]
 proxy_timeGetTime ENDP
 proxy_timeKillEvent PROC
    jmp QWORD PTR [g_winmm_procs + 1112]
 proxy_timeKillEvent ENDP
 proxy_timeSetEvent PROC
    jmp QWORD PTR [g_winmm_procs + 1120]
 proxy_timeSetEvent ENDP
 proxy_waveInAddBuffer PROC
    jmp QWORD PTR [g_winmm_procs + 1128]
 proxy_waveInAddBuffer ENDP
 proxy_waveInClose PROC
    jmp QWORD PTR [g_winmm_procs + 1136]
 proxy_waveInClose ENDP
 proxy_waveInGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 1144]
 proxy_waveInGetDevCapsA ENDP
 proxy_waveInGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 1152]
 proxy_waveInGetDevCapsW ENDP
 proxy_waveInGetErrorTextA PROC
    jmp QWORD PTR [g_winmm_procs + 1160]
 proxy_waveInGetErrorTextA ENDP
 proxy_waveInGetErrorTextW PROC
    jmp QWORD PTR [g_winmm_procs + 1168]
 proxy_waveInGetErrorTextW ENDP
 proxy_waveInGetID PROC
    jmp QWORD PTR [g_winmm_procs + 1176]
 proxy_waveInGetID ENDP
 proxy_waveInGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 1184]
 proxy_waveInGetNumDevs ENDP
 proxy_waveInGetPosition PROC
    jmp QWORD PTR [g_winmm_procs + 1192]
 proxy_waveInGetPosition ENDP
 proxy_waveInMessage PROC
    jmp QWORD PTR [g_winmm_procs + 1200]
 proxy_waveInMessage ENDP
 proxy_waveInOpen PROC
    jmp QWORD PTR [g_winmm_procs + 1208]
 proxy_waveInOpen ENDP
 proxy_waveInPrepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 1216]
 proxy_waveInPrepareHeader ENDP
 proxy_waveInReset PROC
    jmp QWORD PTR [g_winmm_procs + 1224]
 proxy_waveInReset ENDP
 proxy_waveInStart PROC
    jmp QWORD PTR [g_winmm_procs + 1232]
 proxy_waveInStart ENDP
 proxy_waveInStop PROC
    jmp QWORD PTR [g_winmm_procs + 1240]
 proxy_waveInStop ENDP
 proxy_waveInUnprepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 1248]
 proxy_waveInUnprepareHeader ENDP
 proxy_waveOutBreakLoop PROC
    jmp QWORD PTR [g_winmm_procs + 1256]
 proxy_waveOutBreakLoop ENDP
 proxy_waveOutClose PROC
    jmp QWORD PTR [g_winmm_procs + 1264]
 proxy_waveOutClose ENDP
 proxy_waveOutGetDevCapsA PROC
    jmp QWORD PTR [g_winmm_procs + 1272]
 proxy_waveOutGetDevCapsA ENDP
 proxy_waveOutGetDevCapsW PROC
    jmp QWORD PTR [g_winmm_procs + 1280]
 proxy_waveOutGetDevCapsW ENDP
 proxy_waveOutGetErrorTextA PROC
    jmp QWORD PTR [g_winmm_procs + 1288]
 proxy_waveOutGetErrorTextA ENDP
 proxy_waveOutGetErrorTextW PROC
    jmp QWORD PTR [g_winmm_procs + 1296]
 proxy_waveOutGetErrorTextW ENDP
 proxy_waveOutGetID PROC
    jmp QWORD PTR [g_winmm_procs + 1304]
 proxy_waveOutGetID ENDP
 proxy_waveOutGetNumDevs PROC
    jmp QWORD PTR [g_winmm_procs + 1312]
 proxy_waveOutGetNumDevs ENDP
 proxy_waveOutGetPitch PROC
    jmp QWORD PTR [g_winmm_procs + 1320]
 proxy_waveOutGetPitch ENDP
 proxy_waveOutGetPlaybackRate PROC
    jmp QWORD PTR [g_winmm_procs + 1328]
 proxy_waveOutGetPlaybackRate ENDP
 proxy_waveOutGetPosition PROC
    jmp QWORD PTR [g_winmm_procs + 1336]
 proxy_waveOutGetPosition ENDP
 proxy_waveOutGetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 1344]
 proxy_waveOutGetVolume ENDP
 proxy_waveOutMessage PROC
    jmp QWORD PTR [g_winmm_procs + 1352]
 proxy_waveOutMessage ENDP
 proxy_waveOutOpen PROC
    jmp QWORD PTR [g_winmm_procs + 1360]
 proxy_waveOutOpen ENDP
 proxy_waveOutPause PROC
    jmp QWORD PTR [g_winmm_procs + 1368]
 proxy_waveOutPause ENDP
 proxy_waveOutPrepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 1376]
 proxy_waveOutPrepareHeader ENDP
 proxy_waveOutReset PROC
    jmp QWORD PTR [g_winmm_procs + 1384]
 proxy_waveOutReset ENDP
 proxy_waveOutRestart PROC
    jmp QWORD PTR [g_winmm_procs + 1392]
 proxy_waveOutRestart ENDP
 proxy_waveOutSetPitch PROC
    jmp QWORD PTR [g_winmm_procs + 1400]
 proxy_waveOutSetPitch ENDP
 proxy_waveOutSetPlaybackRate PROC
    jmp QWORD PTR [g_winmm_procs + 1408]
 proxy_waveOutSetPlaybackRate ENDP
 proxy_waveOutSetVolume PROC
    jmp QWORD PTR [g_winmm_procs + 1416]
 proxy_waveOutSetVolume ENDP
 proxy_waveOutUnprepareHeader PROC
    jmp QWORD PTR [g_winmm_procs + 1424]
 proxy_waveOutUnprepareHeader ENDP
 proxy_waveOutWrite PROC
    jmp QWORD PTR [g_winmm_procs + 1432]
 proxy_waveOutWrite ENDP
END
