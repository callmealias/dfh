#pragma once

// Windows Native API data types, enumerations, constants, etc...

#include <windows.h>
#include <stdint.h>

typedef LONG NTSTATUS;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define STATUS_SUCCESS 0x00000000
#define STATUS_BUFFER_OVERFLOW 0x80000005
#define STATUS_UNSUCCESSFUL 0xC0000001

typedef struct _IO_STATUS_BLOCK {
  union {
    NTSTATUS Status;
    PVOID    Pointer;
  };
  ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef enum _FILE_INFORMATION_CLASS { 
  FileDirectoryInformation = 1,
  FileFullDirectoryInformation,
  FileBothDirectoryInformation,
  FileBasicInformation,
  FileStandardInformation,
  FileInternalInformation,
  FileEaInformation,
  FileAccessInformation,
  FileNameInformation,
  FileRenameInformation,
  FileLinkInformation,
  FileNamesInformation,
  FileDispositionInformation,
  FilePositionInformation,
  FileFullEaInformation,
  FileModeInformation,
  FileAlignmentInformation,
  FileAllInformation,
  FileAllocationInformation,
  FileEndOfFileInformation,
  FileAlternateNameInformation,
  FileStreamInformation,
  FilePipeInformation,
  FilePipeLocalInformation,
  FilePipeRemoteInformation,
  FileMailslotQueryInformation,
  FileMailslotSetInformation,
  FileCompressionInformation,
  FileObjectIdInformation,
  FileCompletionInformation,
  FileMoveClusterInformation,
  FileQuotaInformation,
  FileReparsePointInformation,
  FileNetworkOpenInformation,
  FileAttributeTagInformation,
  FileTrackingInformation,
  FileIdBothDirectoryInformation,
  FileIdFullDirectoryInformation,
  FileValidDataLengthInformation,
  FileShortNameInformation,
  FileIoCompletionNotificationInformation,
  FileIoStatusBlockRangeInformation,
  FileIoPriorityHintInformation,
  FileSfioReserveInformation,
  FileSfioVolumeInformation,
  FileHardLinkInformation,
  FileProcessIdsUsingFileInformation,
  FileNormalizedNameInformation,
  FileNetworkPhysicalNameInformation,
  FileIdGlobalTxDirectoryInformation,
  FileIsRemoteDeviceInformation,
  FileAttributeCacheInformation,
  FileNumaNodeInformation,
  FileStandardLinkInformation,
  FileRemoteProtocolInformation,
  FileReplaceCompletionInformation,
  FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

#define FILE_NAME_INFORMATION_REQUIRED_SIZE \
    (sizeof(FILE_NAME_INFORMATION) + (sizeof(wchar_t) * (MAX_PATH - 1)))

typedef struct _FILE_NAME_INFORMATION {
  ULONG FileNameLength;
  WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef enum { 
  FileFsVolumeInformation       = 1,
  FileFsLabelInformation        = 2,
  FileFsSizeInformation         = 3,
  FileFsDeviceInformation       = 4,
  FileFsAttributeInformation    = 5,
  FileFsControlInformation      = 6,
  FileFsFullSizeInformation     = 7,
  FileFsObjectIdInformation     = 8,
  FileFsDriverPathInformation   = 9,
  FileFsVolumeFlagsInformation  = 10,
  FileFsSectorSizeInformation   = 11
} FS_INFORMATION_CLASS;

// The maximum volume label length is 32 characters 
#define FILE_FS_VOLUME_INFORMATION_REQUIRED_SIZE \
  (sizeof(FILE_FS_VOLUME_INFORMATION) + (sizeof(wchar_t) * (32)))

typedef struct _FILE_FS_VOLUME_INFORMATION {
  LARGE_INTEGER VolumeCreationTime;
  ULONG         VolumeSerialNumber;
  ULONG         VolumeLabelLength;
  BOOLEAN       SupportsObjects;
  WCHAR         VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef NTSTATUS (WINAPI* _NtSetInformationFile)( 
  _In_   HANDLE FileHandle,
  _Out_  PIO_STATUS_BLOCK IoStatusBlock,
  _In_   PVOID FileInformation,
  _In_   ULONG Length,
  _In_   FILE_INFORMATION_CLASS FileInformationClass
  );

typedef NTSTATUS (WINAPI* _NtQueryVolumeInformationFile)(
  _In_   HANDLE FileHandle,
  _Out_  PIO_STATUS_BLOCK IoStatusBlock,
  _Out_  PVOID FsInformation,
  _In_   ULONG Length,
  _In_   FS_INFORMATION_CLASS FsInformationClass
  );

typedef NTSTATUS (WINAPI* _NtQueryInformationFile)(
  _In_   HANDLE FileHandle,
  _Out_  PIO_STATUS_BLOCK IoStatusBlock,
  _Out_  PVOID FileInformation,
  _In_   ULONG Length,
  _In_   FILE_INFORMATION_CLASS FileInformationClass
  );