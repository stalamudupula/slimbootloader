/** @file

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include  <FspApiLibInternal.h>

/**
  This FSP API is called after FspMemoryInit API. This FSP API tears down the temporary
  memory setup by TempRamInit API. This FSP API accepts a pointer to a data structure
  that will be platform dependent and defined for each FSP binary. This will be
  documented in Integration Guide.
  FspMemoryInit, TempRamExit and FspSiliconInit APIs provide an alternate method to
  complete the silicon initialization and provides bootloader an opportunity to get
  control after system memory is available and before the temporary RAM is torn down.

  @param[in] FspmBase            The base address of FSPM.
  @param[in] Params              Pointer to the Temp Ram Exit parameters structure.
                                 This structure is normally defined in the Integration Guide.
                                 And if it is not defined in the Integration Guide, pass NULL.

  @retval EFI_SUCCESS            FSP execution environment was initialized successfully.
  @retval EFI_INVALID_PARAMETER  Input parameters are invalid.
  @retval EFI_UNSUPPORTED        The FSP calling conditions were not met.
  @retval EFI_DEVICE_ERROR       FSP initialization failed.
**/
EFI_STATUS
EFIAPI
CallFspTempRamExit (
  UINT32               FspmBase,
  VOID                *Params
  )
{
  FSP_TEMP_RAM_EXIT   TempRamExit;
  FSP_INFO_HEADER    *FspHeader;
  EFI_STATUS          Status;

  FspHeader = (FSP_INFO_HEADER *) (FspmBase + FSP_INFO_HEADER_OFF);
  if (FspHeader->TempRamExitEntryOffset == 0) {
    return EFI_UNSUPPORTED;
  }

  TempRamExit = (FSP_TEMP_RAM_EXIT) (FspHeader->ImageBase + FspHeader->TempRamExitEntryOffset);

  DEBUG ((DEBUG_INFO, "Call FspTempRamExit ... "));
  Status  = TempRamExit (NULL);
  DEBUG ((DEBUG_INFO, "%r\n", Status));

  return Status;
}
