EXTERN g_proxy_procs:QWORD
.code
 proxy_PdhAddCounterA PROC
    jmp QWORD PTR [g_proxy_procs + 0]
 proxy_PdhAddCounterA ENDP
 proxy_PdhAddCounterW PROC
    jmp QWORD PTR [g_proxy_procs + 8]
 proxy_PdhAddCounterW ENDP
 proxy_PdhAddEnglishCounterA PROC
    jmp QWORD PTR [g_proxy_procs + 16]
 proxy_PdhAddEnglishCounterA ENDP
 proxy_PdhAddEnglishCounterW PROC
    jmp QWORD PTR [g_proxy_procs + 24]
 proxy_PdhAddEnglishCounterW ENDP
 proxy_PdhAddRelogCounter PROC
    jmp QWORD PTR [g_proxy_procs + 32]
 proxy_PdhAddRelogCounter ENDP
 proxy_PdhBindInputDataSourceA PROC
    jmp QWORD PTR [g_proxy_procs + 40]
 proxy_PdhBindInputDataSourceA ENDP
 proxy_PdhBindInputDataSourceW PROC
    jmp QWORD PTR [g_proxy_procs + 48]
 proxy_PdhBindInputDataSourceW ENDP
 proxy_PdhBrowseCountersA PROC
    jmp QWORD PTR [g_proxy_procs + 56]
 proxy_PdhBrowseCountersA ENDP
 proxy_PdhBrowseCountersHA PROC
    jmp QWORD PTR [g_proxy_procs + 64]
 proxy_PdhBrowseCountersHA ENDP
 proxy_PdhBrowseCountersHW PROC
    jmp QWORD PTR [g_proxy_procs + 72]
 proxy_PdhBrowseCountersHW ENDP
 proxy_PdhBrowseCountersW PROC
    jmp QWORD PTR [g_proxy_procs + 80]
 proxy_PdhBrowseCountersW ENDP
 proxy_PdhCalculateCounterFromRawValue PROC
    jmp QWORD PTR [g_proxy_procs + 88]
 proxy_PdhCalculateCounterFromRawValue ENDP
 proxy_PdhCloseLog PROC
    jmp QWORD PTR [g_proxy_procs + 96]
 proxy_PdhCloseLog ENDP
 proxy_PdhCloseQuery PROC
    jmp QWORD PTR [g_proxy_procs + 104]
 proxy_PdhCloseQuery ENDP
 proxy_PdhCollectQueryData PROC
    jmp QWORD PTR [g_proxy_procs + 112]
 proxy_PdhCollectQueryData ENDP
 proxy_PdhCollectQueryDataEx PROC
    jmp QWORD PTR [g_proxy_procs + 120]
 proxy_PdhCollectQueryDataEx ENDP
 proxy_PdhCollectQueryDataWithTime PROC
    jmp QWORD PTR [g_proxy_procs + 128]
 proxy_PdhCollectQueryDataWithTime ENDP
 proxy_PdhComputeCounterStatistics PROC
    jmp QWORD PTR [g_proxy_procs + 136]
 proxy_PdhComputeCounterStatistics ENDP
 proxy_PdhConnectMachineA PROC
    jmp QWORD PTR [g_proxy_procs + 144]
 proxy_PdhConnectMachineA ENDP
 proxy_PdhConnectMachineW PROC
    jmp QWORD PTR [g_proxy_procs + 152]
 proxy_PdhConnectMachineW ENDP
 proxy_PdhCreateSQLTablesA PROC
    jmp QWORD PTR [g_proxy_procs + 160]
 proxy_PdhCreateSQLTablesA ENDP
 proxy_PdhCreateSQLTablesW PROC
    jmp QWORD PTR [g_proxy_procs + 168]
 proxy_PdhCreateSQLTablesW ENDP
 proxy_PdhEnumLogSetNamesA PROC
    jmp QWORD PTR [g_proxy_procs + 176]
 proxy_PdhEnumLogSetNamesA ENDP
 proxy_PdhEnumLogSetNamesW PROC
    jmp QWORD PTR [g_proxy_procs + 184]
 proxy_PdhEnumLogSetNamesW ENDP
 proxy_PdhEnumMachinesA PROC
    jmp QWORD PTR [g_proxy_procs + 192]
 proxy_PdhEnumMachinesA ENDP
 proxy_PdhEnumMachinesHA PROC
    jmp QWORD PTR [g_proxy_procs + 200]
 proxy_PdhEnumMachinesHA ENDP
 proxy_PdhEnumMachinesHW PROC
    jmp QWORD PTR [g_proxy_procs + 208]
 proxy_PdhEnumMachinesHW ENDP
 proxy_PdhEnumMachinesW PROC
    jmp QWORD PTR [g_proxy_procs + 216]
 proxy_PdhEnumMachinesW ENDP
 proxy_PdhEnumObjectItemsA PROC
    jmp QWORD PTR [g_proxy_procs + 224]
 proxy_PdhEnumObjectItemsA ENDP
 proxy_PdhEnumObjectItemsHA PROC
    jmp QWORD PTR [g_proxy_procs + 232]
 proxy_PdhEnumObjectItemsHA ENDP
 proxy_PdhEnumObjectItemsHW PROC
    jmp QWORD PTR [g_proxy_procs + 240]
 proxy_PdhEnumObjectItemsHW ENDP
 proxy_PdhEnumObjectItemsW PROC
    jmp QWORD PTR [g_proxy_procs + 248]
 proxy_PdhEnumObjectItemsW ENDP
 proxy_PdhEnumObjectsA PROC
    jmp QWORD PTR [g_proxy_procs + 256]
 proxy_PdhEnumObjectsA ENDP
 proxy_PdhEnumObjectsHA PROC
    jmp QWORD PTR [g_proxy_procs + 264]
 proxy_PdhEnumObjectsHA ENDP
 proxy_PdhEnumObjectsHW PROC
    jmp QWORD PTR [g_proxy_procs + 272]
 proxy_PdhEnumObjectsHW ENDP
 proxy_PdhEnumObjectsW PROC
    jmp QWORD PTR [g_proxy_procs + 280]
 proxy_PdhEnumObjectsW ENDP
 proxy_PdhExpandCounterPathA PROC
    jmp QWORD PTR [g_proxy_procs + 288]
 proxy_PdhExpandCounterPathA ENDP
 proxy_PdhExpandCounterPathW PROC
    jmp QWORD PTR [g_proxy_procs + 296]
 proxy_PdhExpandCounterPathW ENDP
 proxy_PdhExpandWildCardPathA PROC
    jmp QWORD PTR [g_proxy_procs + 304]
 proxy_PdhExpandWildCardPathA ENDP
 proxy_PdhExpandWildCardPathHA PROC
    jmp QWORD PTR [g_proxy_procs + 312]
 proxy_PdhExpandWildCardPathHA ENDP
 proxy_PdhExpandWildCardPathHW PROC
    jmp QWORD PTR [g_proxy_procs + 320]
 proxy_PdhExpandWildCardPathHW ENDP
 proxy_PdhExpandWildCardPathW PROC
    jmp QWORD PTR [g_proxy_procs + 328]
 proxy_PdhExpandWildCardPathW ENDP
 proxy_PdhFormatFromRawValue PROC
    jmp QWORD PTR [g_proxy_procs + 336]
 proxy_PdhFormatFromRawValue ENDP
 proxy_PdhGetCounterInfoA PROC
    jmp QWORD PTR [g_proxy_procs + 344]
 proxy_PdhGetCounterInfoA ENDP
 proxy_PdhGetCounterInfoW PROC
    jmp QWORD PTR [g_proxy_procs + 352]
 proxy_PdhGetCounterInfoW ENDP
 proxy_PdhGetCounterTimeBase PROC
    jmp QWORD PTR [g_proxy_procs + 360]
 proxy_PdhGetCounterTimeBase ENDP
 proxy_PdhGetDataSourceTimeRangeA PROC
    jmp QWORD PTR [g_proxy_procs + 368]
 proxy_PdhGetDataSourceTimeRangeA ENDP
 proxy_PdhGetDataSourceTimeRangeH PROC
    jmp QWORD PTR [g_proxy_procs + 376]
 proxy_PdhGetDataSourceTimeRangeH ENDP
 proxy_PdhGetDataSourceTimeRangeW PROC
    jmp QWORD PTR [g_proxy_procs + 384]
 proxy_PdhGetDataSourceTimeRangeW ENDP
 proxy_PdhGetDefaultPerfCounterA PROC
    jmp QWORD PTR [g_proxy_procs + 392]
 proxy_PdhGetDefaultPerfCounterA ENDP
 proxy_PdhGetDefaultPerfCounterHA PROC
    jmp QWORD PTR [g_proxy_procs + 400]
 proxy_PdhGetDefaultPerfCounterHA ENDP
 proxy_PdhGetDefaultPerfCounterHW PROC
    jmp QWORD PTR [g_proxy_procs + 408]
 proxy_PdhGetDefaultPerfCounterHW ENDP
 proxy_PdhGetDefaultPerfCounterW PROC
    jmp QWORD PTR [g_proxy_procs + 416]
 proxy_PdhGetDefaultPerfCounterW ENDP
 proxy_PdhGetDefaultPerfObjectA PROC
    jmp QWORD PTR [g_proxy_procs + 424]
 proxy_PdhGetDefaultPerfObjectA ENDP
 proxy_PdhGetDefaultPerfObjectHA PROC
    jmp QWORD PTR [g_proxy_procs + 432]
 proxy_PdhGetDefaultPerfObjectHA ENDP
 proxy_PdhGetDefaultPerfObjectHW PROC
    jmp QWORD PTR [g_proxy_procs + 440]
 proxy_PdhGetDefaultPerfObjectHW ENDP
 proxy_PdhGetDefaultPerfObjectW PROC
    jmp QWORD PTR [g_proxy_procs + 448]
 proxy_PdhGetDefaultPerfObjectW ENDP
 proxy_PdhGetDllVersion PROC
    jmp QWORD PTR [g_proxy_procs + 456]
 proxy_PdhGetDllVersion ENDP
 proxy_PdhGetExplainText PROC
    jmp QWORD PTR [g_proxy_procs + 464]
 proxy_PdhGetExplainText ENDP
 proxy_PdhGetFormattedCounterArrayA PROC
    jmp QWORD PTR [g_proxy_procs + 472]
 proxy_PdhGetFormattedCounterArrayA ENDP
 proxy_PdhGetFormattedCounterArrayW PROC
    jmp QWORD PTR [g_proxy_procs + 480]
 proxy_PdhGetFormattedCounterArrayW ENDP
 proxy_PdhGetFormattedCounterValue PROC
    jmp QWORD PTR [g_proxy_procs + 488]
 proxy_PdhGetFormattedCounterValue ENDP
 proxy_PdhGetLogFileSize PROC
    jmp QWORD PTR [g_proxy_procs + 496]
 proxy_PdhGetLogFileSize ENDP
 proxy_PdhGetLogFileTypeW PROC
    jmp QWORD PTR [g_proxy_procs + 504]
 proxy_PdhGetLogFileTypeW ENDP
 proxy_PdhGetLogSetGUID PROC
    jmp QWORD PTR [g_proxy_procs + 512]
 proxy_PdhGetLogSetGUID ENDP
 proxy_PdhGetRawCounterArrayA PROC
    jmp QWORD PTR [g_proxy_procs + 520]
 proxy_PdhGetRawCounterArrayA ENDP
 proxy_PdhGetRawCounterArrayW PROC
    jmp QWORD PTR [g_proxy_procs + 528]
 proxy_PdhGetRawCounterArrayW ENDP
 proxy_PdhGetRawCounterValue PROC
    jmp QWORD PTR [g_proxy_procs + 536]
 proxy_PdhGetRawCounterValue ENDP
 proxy_PdhIsRealTimeQuery PROC
    jmp QWORD PTR [g_proxy_procs + 544]
 proxy_PdhIsRealTimeQuery ENDP
 proxy_PdhLookupPerfIndexByNameA PROC
    jmp QWORD PTR [g_proxy_procs + 552]
 proxy_PdhLookupPerfIndexByNameA ENDP
 proxy_PdhLookupPerfIndexByNameW PROC
    jmp QWORD PTR [g_proxy_procs + 560]
 proxy_PdhLookupPerfIndexByNameW ENDP
 proxy_PdhLookupPerfNameByIndexA PROC
    jmp QWORD PTR [g_proxy_procs + 568]
 proxy_PdhLookupPerfNameByIndexA ENDP
 proxy_PdhLookupPerfNameByIndexW PROC
    jmp QWORD PTR [g_proxy_procs + 576]
 proxy_PdhLookupPerfNameByIndexW ENDP
 proxy_PdhMakeCounterPathA PROC
    jmp QWORD PTR [g_proxy_procs + 584]
 proxy_PdhMakeCounterPathA ENDP
 proxy_PdhMakeCounterPathW PROC
    jmp QWORD PTR [g_proxy_procs + 592]
 proxy_PdhMakeCounterPathW ENDP
 proxy_PdhOpenLogA PROC
    jmp QWORD PTR [g_proxy_procs + 600]
 proxy_PdhOpenLogA ENDP
 proxy_PdhOpenLogW PROC
    jmp QWORD PTR [g_proxy_procs + 608]
 proxy_PdhOpenLogW ENDP
 proxy_PdhOpenQuery PROC
    jmp QWORD PTR [g_proxy_procs + 616]
 proxy_PdhOpenQuery ENDP
 proxy_PdhOpenQueryA PROC
    jmp QWORD PTR [g_proxy_procs + 624]
 proxy_PdhOpenQueryA ENDP
 proxy_PdhOpenQueryH PROC
    jmp QWORD PTR [g_proxy_procs + 632]
 proxy_PdhOpenQueryH ENDP
 proxy_PdhOpenQueryW PROC
    jmp QWORD PTR [g_proxy_procs + 640]
 proxy_PdhOpenQueryW ENDP
 proxy_PdhParseCounterPathA PROC
    jmp QWORD PTR [g_proxy_procs + 648]
 proxy_PdhParseCounterPathA ENDP
 proxy_PdhParseCounterPathW PROC
    jmp QWORD PTR [g_proxy_procs + 656]
 proxy_PdhParseCounterPathW ENDP
 proxy_PdhParseInstanceNameA PROC
    jmp QWORD PTR [g_proxy_procs + 664]
 proxy_PdhParseInstanceNameA ENDP
 proxy_PdhParseInstanceNameW PROC
    jmp QWORD PTR [g_proxy_procs + 672]
 proxy_PdhParseInstanceNameW ENDP
 proxy_PdhReadRawLogRecord PROC
    jmp QWORD PTR [g_proxy_procs + 680]
 proxy_PdhReadRawLogRecord ENDP
 proxy_PdhRelogW PROC
    jmp QWORD PTR [g_proxy_procs + 688]
 proxy_PdhRelogW ENDP
 proxy_PdhRemoveCounter PROC
    jmp QWORD PTR [g_proxy_procs + 696]
 proxy_PdhRemoveCounter ENDP
 proxy_PdhResetRelogCounterValues PROC
    jmp QWORD PTR [g_proxy_procs + 704]
 proxy_PdhResetRelogCounterValues ENDP
 proxy_PdhSelectDataSourceA PROC
    jmp QWORD PTR [g_proxy_procs + 712]
 proxy_PdhSelectDataSourceA ENDP
 proxy_PdhSelectDataSourceW PROC
    jmp QWORD PTR [g_proxy_procs + 720]
 proxy_PdhSelectDataSourceW ENDP
 proxy_PdhSetCounterScaleFactor PROC
    jmp QWORD PTR [g_proxy_procs + 728]
 proxy_PdhSetCounterScaleFactor ENDP
 proxy_PdhSetCounterValue PROC
    jmp QWORD PTR [g_proxy_procs + 736]
 proxy_PdhSetCounterValue ENDP
 proxy_PdhSetDefaultRealTimeDataSource PROC
    jmp QWORD PTR [g_proxy_procs + 744]
 proxy_PdhSetDefaultRealTimeDataSource ENDP
 proxy_PdhSetLogSetRunID PROC
    jmp QWORD PTR [g_proxy_procs + 752]
 proxy_PdhSetLogSetRunID ENDP
 proxy_PdhSetQueryTimeRange PROC
    jmp QWORD PTR [g_proxy_procs + 760]
 proxy_PdhSetQueryTimeRange ENDP
 proxy_PdhTranslate009CounterW PROC
    jmp QWORD PTR [g_proxy_procs + 768]
 proxy_PdhTranslate009CounterW ENDP
 proxy_PdhTranslateLocaleCounterW PROC
    jmp QWORD PTR [g_proxy_procs + 776]
 proxy_PdhTranslateLocaleCounterW ENDP
 proxy_PdhUpdateLogA PROC
    jmp QWORD PTR [g_proxy_procs + 784]
 proxy_PdhUpdateLogA ENDP
 proxy_PdhUpdateLogFileCatalog PROC
    jmp QWORD PTR [g_proxy_procs + 792]
 proxy_PdhUpdateLogFileCatalog ENDP
 proxy_PdhUpdateLogW PROC
    jmp QWORD PTR [g_proxy_procs + 800]
 proxy_PdhUpdateLogW ENDP
 proxy_PdhValidatePathA PROC
    jmp QWORD PTR [g_proxy_procs + 808]
 proxy_PdhValidatePathA ENDP
 proxy_PdhValidatePathExA PROC
    jmp QWORD PTR [g_proxy_procs + 816]
 proxy_PdhValidatePathExA ENDP
 proxy_PdhValidatePathExW PROC
    jmp QWORD PTR [g_proxy_procs + 824]
 proxy_PdhValidatePathExW ENDP
 proxy_PdhValidatePathW PROC
    jmp QWORD PTR [g_proxy_procs + 832]
 proxy_PdhValidatePathW ENDP
 proxy_PdhVbAddCounter PROC
    jmp QWORD PTR [g_proxy_procs + 840]
 proxy_PdhVbAddCounter ENDP
 proxy_PdhVbCreateCounterPathList PROC
    jmp QWORD PTR [g_proxy_procs + 848]
 proxy_PdhVbCreateCounterPathList ENDP
 proxy_PdhVbGetCounterPathElements PROC
    jmp QWORD PTR [g_proxy_procs + 856]
 proxy_PdhVbGetCounterPathElements ENDP
 proxy_PdhVbGetCounterPathFromList PROC
    jmp QWORD PTR [g_proxy_procs + 864]
 proxy_PdhVbGetCounterPathFromList ENDP
 proxy_PdhVbGetDoubleCounterValue PROC
    jmp QWORD PTR [g_proxy_procs + 872]
 proxy_PdhVbGetDoubleCounterValue ENDP
 proxy_PdhVbGetLogFileSize PROC
    jmp QWORD PTR [g_proxy_procs + 880]
 proxy_PdhVbGetLogFileSize ENDP
 proxy_PdhVbGetOneCounterPath PROC
    jmp QWORD PTR [g_proxy_procs + 888]
 proxy_PdhVbGetOneCounterPath ENDP
 proxy_PdhVbIsGoodStatus PROC
    jmp QWORD PTR [g_proxy_procs + 896]
 proxy_PdhVbIsGoodStatus ENDP
 proxy_PdhVbOpenLog PROC
    jmp QWORD PTR [g_proxy_procs + 904]
 proxy_PdhVbOpenLog ENDP
 proxy_PdhVbOpenQuery PROC
    jmp QWORD PTR [g_proxy_procs + 912]
 proxy_PdhVbOpenQuery ENDP
 proxy_PdhVbUpdateLog PROC
    jmp QWORD PTR [g_proxy_procs + 920]
 proxy_PdhVbUpdateLog ENDP
 proxy_PdhVerifySQLDBA PROC
    jmp QWORD PTR [g_proxy_procs + 928]
 proxy_PdhVerifySQLDBA ENDP
 proxy_PdhVerifySQLDBW PROC
    jmp QWORD PTR [g_proxy_procs + 936]
 proxy_PdhVerifySQLDBW ENDP
 proxy_PdhWriteRelogSample PROC
    jmp QWORD PTR [g_proxy_procs + 944]
 proxy_PdhWriteRelogSample ENDP
END
