/*
 *  ReactOS W32 Subsystem
 *  Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS kernel
 * PURPOSE:          Messages
 * FILE:             subsys/win32k/ntuser/keyboard.c
 * PROGRAMER:        Casper S. Hornstrup (chorns@users.sourceforge.net)
 * REVISION HISTORY:
 *       06-06-2001  CSH  Created
 */

/* INCLUDES ******************************************************************/

#include <win32k.h>
#define NDEBUG
#include <debug.h>


/* Lock modifiers */
#define CAPITAL_BIT   0x80000000
#define NUMLOCK_BIT   0x40000000
#define MOD_BITS_MASK 0x3fffffff
/* Key States */
#define KEY_DOWN_BIT      0x80
#define KEY_LOCK_BIT    0x01


BYTE gQueueKeyStateTable[256];


static UINT DontDistinguishShifts( UINT ret )
{
   if( ret == VK_LSHIFT || ret == VK_RSHIFT )
      ret = VK_LSHIFT;
   if( ret == VK_LCONTROL || ret == VK_RCONTROL )
      ret = VK_LCONTROL;
   if( ret == VK_LMENU || ret == VK_RMENU )
      ret = VK_LMENU;
   return ret;
}

VOID DumpKeyState( PBYTE KeyState )
{
   int i;

   DbgPrint( "KeyState { " );
   for( i = 0; i < 0x100; i++ )
   {
      if( KeyState[i] )
         DbgPrint( "%02x(%02x) ", i, KeyState[i] );
   }
   DbgPrint( "};\n" );
}

static BYTE KeysSet( PKBDTABLES pkKT, PBYTE KeyState,
                     int FakeModLeft, int FakeModRight )
{
   if( !KeyState || !pkKT )
      return 0;

   /* Search special codes first */
   if( FakeModLeft && KeyState[FakeModLeft] )
      return KeyState[FakeModLeft];
   else if( FakeModRight && KeyState[FakeModRight] )
      return KeyState[FakeModRight];

   return 0;
}

/* Search the keyboard layout modifiers table for the shift bit.  I don't
 * want to count on the shift bit not moving, because it can be specified
 * in the layout */

static DWORD FASTCALL GetShiftBit( PKBDTABLES pkKT, DWORD Vk )
{
   int i;

   for( i = 0; pkKT->pCharModifiers->pVkToBit[i].Vk; i++ )
      if( pkKT->pCharModifiers->pVkToBit[i].Vk == Vk )
         return pkKT->pCharModifiers->pVkToBit[i].ModBits;

   return 0;
}

static DWORD ModBits( PKBDTABLES pkKT, PBYTE KeyState )
{
   DWORD ModBits = 0;

   if( !KeyState )
      return 0;

   /* DumpKeyState( KeyState ); */

   if (KeysSet( pkKT, KeyState, VK_LSHIFT, VK_RSHIFT ) &
         KEY_DOWN_BIT)
      ModBits |= GetShiftBit( pkKT, VK_SHIFT );

   if (KeysSet( pkKT, KeyState, VK_SHIFT, 0 ) &
         KEY_DOWN_BIT)
      ModBits |= GetShiftBit( pkKT, VK_SHIFT );

   if (KeysSet( pkKT, KeyState, VK_LCONTROL, VK_RCONTROL ) &
         KEY_DOWN_BIT )
      ModBits |= GetShiftBit( pkKT, VK_CONTROL );

   if (KeysSet( pkKT, KeyState, VK_CONTROL, 0 ) &
         KEY_DOWN_BIT )
      ModBits |= GetShiftBit( pkKT, VK_CONTROL );

   if (KeysSet( pkKT, KeyState, VK_LMENU, VK_RMENU ) &
         KEY_DOWN_BIT )
      ModBits |= GetShiftBit( pkKT, VK_MENU );

   /* Handle Alt+Gr */
   if (pkKT->fLocaleFlags & 0x1) 
      if (KeysSet( pkKT, KeyState, VK_RMENU, 0 ) &
            KEY_DOWN_BIT)
         ModBits |= GetShiftBit( pkKT, VK_CONTROL );

   /* Deal with VK_CAPITAL */
   if (KeysSet( pkKT, KeyState, VK_CAPITAL, 0 ) & KEY_LOCK_BIT)
   {
      ModBits |= CAPITAL_BIT;
   }

   /* Deal with VK_NUMLOCK */
   if (KeysSet( pkKT, KeyState, VK_NUMLOCK, 0 ) & KEY_LOCK_BIT)
   {
      ModBits |= NUMLOCK_BIT;
   }

   DPRINT( "Current Mod Bits: %x\n", ModBits );

   return ModBits;
}

static BOOL TryToTranslateChar(WORD wVirtKey,
                               DWORD ModBits,
                               PBOOL pbDead,
                               PBOOL pbLigature,
                               PWCHAR pwcTranslatedChar,
                               PKBDTABLES keyLayout )
{
   PVK_TO_WCHAR_TABLE vtwTbl;
   PVK_TO_WCHARS10 vkPtr;
   size_t size_this_entry;
   int nMod;
   DWORD CapsMod = 0, CapsState = 0;

   CapsState = ModBits & ~MOD_BITS_MASK;
   ModBits = ModBits & MOD_BITS_MASK;

   DPRINT ( "TryToTranslate: %04x %x\n", wVirtKey, ModBits );

   if (ModBits > keyLayout->pCharModifiers->wMaxModBits)
   {
      return FALSE;
   }

   for (nMod = 0; keyLayout->pVkToWcharTable[nMod].nModifications; nMod++)
   {
      vtwTbl = &keyLayout->pVkToWcharTable[nMod];
      size_this_entry = vtwTbl->cbSize;
      vkPtr = (PVK_TO_WCHARS10)((BYTE *)vtwTbl->pVkToWchars);
      while(vkPtr->VirtualKey)
      {
         if( wVirtKey == (vkPtr->VirtualKey & 0xff) )
         {
            CapsMod = keyLayout->pCharModifiers->ModNumber
                      [ModBits ^
                       ((CapsState & CAPITAL_BIT) ? vkPtr->Attributes : 0)];

            if( CapsMod >= keyLayout->pVkToWcharTable[nMod].nModifications )
            {
               return FALSE;
            }

            if( vkPtr->wch[CapsMod] == WCH_NONE )
            {
               return FALSE;
            }

            *pbDead = vkPtr->wch[CapsMod] == WCH_DEAD;
            *pbLigature = vkPtr->wch[CapsMod] == WCH_LGTR;
            *pwcTranslatedChar = vkPtr->wch[CapsMod];

            DPRINT("%d %04x: CapsMod %08x CapsState %08x Char %04x\n",
                   nMod, wVirtKey,
                   CapsMod, CapsState, *pwcTranslatedChar);

            if( *pbDead )
            {
               vkPtr = (PVK_TO_WCHARS10)(((BYTE *)vkPtr) + size_this_entry);
               if( vkPtr->VirtualKey != 0xff )
               {
                  DPRINT( "Found dead key with no trailer in the table.\n" );
                  DPRINT( "VK: %04x, ADDR: %p\n", wVirtKey, vkPtr );
                  return FALSE;
               }
               *pwcTranslatedChar = vkPtr->wch[CapsMod];
            }
            return TRUE;
         }
         vkPtr = (PVK_TO_WCHARS10)(((BYTE *)vkPtr) + size_this_entry);
      }
   }
   return FALSE;
}

static
int APIENTRY
ToUnicodeInner(UINT wVirtKey,
               UINT wScanCode,
               PBYTE lpKeyState,
               LPWSTR pwszBuff,
               int cchBuff,
               UINT wFlags,
               PKBDTABLES pkKT)
{
   WCHAR wcTranslatedChar;
   BOOL bDead;
   BOOL bLigature;

   if( !pkKT )
      return 0;

   if( TryToTranslateChar( wVirtKey,
                           ModBits( pkKT, lpKeyState ),
                           &bDead,
                           &bLigature,
                           &wcTranslatedChar,
                           pkKT ) )
   {
      if( bLigature )
      {
         DPRINT("Not handling ligature (yet)\n" );
         return 0;
      }

      if( cchBuff > 0 )
         pwszBuff[0] = wcTranslatedChar;

      return bDead ? -1 : 1;
   }

   return 0;
}

DWORD FASTCALL UserGetAsyncKeyState(DWORD key)
{
   DWORD ret = 0;

   if( key < 0x100 )
   {
      ret = ((DWORD)(gQueueKeyStateTable[key] & KEY_DOWN_BIT) << 8 ) |
            (gQueueKeyStateTable[key] & KEY_LOCK_BIT);
   }

   return ret;
}



SHORT
APIENTRY
RosUserGetAsyncKeyState(
   INT key)
{
   SHORT ret;

   UserEnterExclusive();

   ret = (SHORT)UserGetAsyncKeyState(key);

   UserLeave();

   return ret;
}

static UINT VkToScan( UINT Code, BOOL ExtCode, PKBDTABLES pkKT )
{
   int i;

   for( i = 0; i < pkKT->bMaxVSCtoVK; i++ )
   {
      if( pkKT->pusVSCtoVK[i] == Code )
      {
         return i;
      }
   }

   return 0;
}

UINT ScanToVk( UINT Code, BOOL ExtKey, PKBDTABLES pkKT )
{
   if( !pkKT )
   {
      DPRINT("ScanToVk: No layout\n");
      return 0;
   }

   if( ExtKey )
   {
      int i;

      for( i = 0; pkKT->pVSCtoVK_E0[i].Vsc; i++ )
      {
         if( pkKT->pVSCtoVK_E0[i].Vsc == Code )
            return pkKT->pVSCtoVK_E0[i].Vk & 0xff;
      }
      for( i = 0; pkKT->pVSCtoVK_E1[i].Vsc; i++ )
      {
         if( pkKT->pVSCtoVK_E1[i].Vsc == Code )
            return pkKT->pVSCtoVK_E1[i].Vk & 0xff;
      }

      return 0;
   }
   else
   {
      if( Code >= pkKT->bMaxVSCtoVK )
      {
         return 0;
      }
      return pkKT->pusVSCtoVK[Code] & 0xff;
   }
}

/*
 * Map a virtual key code, or virtual scan code, to a scan code, key code,
 * or unshifted unicode character.
 *
 * Code: See Below
 * Type:
 * 0 -- Code is a virtual key code that is converted into a virtual scan code
 *      that does not distinguish between left and right shift keys.
 * 1 -- Code is a virtual scan code that is converted into a virtual key code
 *      that does not distinguish between left and right shift keys.
 * 2 -- Code is a virtual key code that is converted into an unshifted unicode
 *      character.
 * 3 -- Code is a virtual scan code that is converted into a virtual key code
 *      that distinguishes left and right shift keys.
 * KeyLayout: Keyboard layout handle (currently, unused)
 *
 * @implemented
 */

static UINT IntMapVirtualKeyEx( UINT Code, UINT Type, PKBDTABLES keyLayout )
{
   UINT ret = 0;

   switch( Type )
   {
      case 0:
         if( Code == VK_RSHIFT )
            Code = VK_LSHIFT;
         if( Code == VK_RMENU )
            Code = VK_LMENU;
         if( Code == VK_RCONTROL )
            Code = VK_LCONTROL;
         ret = VkToScan( Code, FALSE, keyLayout );
         break;

      case 1:
         ret =
            DontDistinguishShifts
            (IntMapVirtualKeyEx( Code, 3, keyLayout ) );
         break;

      case 2:
         {
            WCHAR wp[2] = {0};

            ret = VkToScan( Code, FALSE, keyLayout );
            ToUnicodeInner( Code, ret, 0, wp, 2, 0, keyLayout );
            ret = wp[0];
         }
         break;

      case 3:

         ret = ScanToVk( Code, FALSE, keyLayout );
         break;
   }

   return ret;
}

UINT
APIENTRY
RosUserMapVirtualKeyEx( UINT Code, UINT Type, DWORD keyboardId, HKL dwhkl )
{
   PTHREADINFO pti;
   PKBDTABLES keyLayout;
   UINT ret = 0;

   UserEnterExclusive();

   pti = PsGetCurrentThreadWin32Thread();
   keyLayout = pti ? pti->KeyboardLayout->KBTables : 0;

	if( keyLayout )
	{
		ret = IntMapVirtualKeyEx( Code, Type, keyLayout );
	}

   UserLeave();

   return ret;
}


int
APIENTRY
RosUserToUnicodeEx(
   UINT wVirtKey,
   UINT wScanCode,
   PBYTE lpKeyState,
   LPWSTR pwszBuff,
   int cchBuff,
   UINT wFlags,
   HKL dwhkl )
{
   PTHREADINFO pti;
   BYTE KeyStateBuf[0x100];
   PWCHAR OutPwszBuff = 0;
   int ret = 0;

   //UserEnterShared();

    NTSTATUS Status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        ProbeForRead(lpKeyState,sizeof(KeyStateBuf),1);
        RtlCopyMemory(KeyStateBuf,lpKeyState,sizeof(KeyStateBuf));
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        Status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;

	if( !NT_SUCCESS(Status))
	{
		goto cleanup;
	}
   
   /* Virtual code is correct and key is pressed currently? */
   if (wVirtKey < 0x100 && KeyStateBuf[wVirtKey] & KEY_DOWN_BIT)
   {
      OutPwszBuff = ExAllocatePoolWithTag(NonPagedPool,sizeof(WCHAR) * cchBuff, TAG_STRING);
      if( !OutPwszBuff )
      {
         DPRINT1( "ExAllocatePool(%d) failed\n", sizeof(WCHAR) * cchBuff);
         goto cleanup;
      }
      RtlZeroMemory( OutPwszBuff, sizeof( WCHAR ) * cchBuff );

      pti = PsGetCurrentThreadWin32Thread();
      ret = ToUnicodeInner( wVirtKey,
                            wScanCode,
                            KeyStateBuf,
                            OutPwszBuff,
                            cchBuff,
                            wFlags,
                            pti ? pti->KeyboardLayout->KBTables : 0 );

	_SEH2_TRY
    {
        ProbeForWrite(pwszBuff,sizeof(WCHAR)*cchBuff,1);
        RtlCopyMemory(pwszBuff,OutPwszBuff,sizeof(WCHAR)*cchBuff);
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        Status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;


      ExFreePoolWithTag(OutPwszBuff, TAG_STRING);
   }

cleanup:
   //UserLeave();
   return ret;
}

//static int W32kSimpleToupper( int ch )
//{
//   if( ch >= 'a' && ch <= 'z' )
//      ch = ch - 'a' + 'A';
//   return ch;
//}

DWORD
APIENTRY
RosUserGetKeyNameText( LONG lParam, LPWSTR lpString, int nSize )
{
   PTHREADINFO pti;
   //int i;
   DWORD ret = 0;
   UINT CareVk = 0;
   UINT VkCode = 0;
   UINT ScanCode = (lParam >> 16) & 0xff;
   BOOL ExtKey = lParam & (1<<24) ? TRUE : FALSE;
   PKBDTABLES keyLayout;
   VSC_LPWSTR *KeyNames;

   //UserEnterShared();

   pti = PsGetCurrentThreadWin32Thread();
   keyLayout = pti ? pti->KeyboardLayout->KBTables : 0;

   if( !keyLayout || nSize < 1 )
      goto cleanup;

   if( lParam & (1<<25) )
   {
      CareVk = VkCode = ScanToVk( ScanCode, ExtKey, keyLayout );
      if( VkCode == VK_LSHIFT || VkCode == VK_RSHIFT )
         VkCode = VK_LSHIFT;
      if( VkCode == VK_LCONTROL || VkCode == VK_RCONTROL )
         VkCode = VK_LCONTROL;
      if( VkCode == VK_LMENU || VkCode == VK_RMENU )
         VkCode = VK_LMENU;
   }
   else
   {
      VkCode = ScanToVk( ScanCode, ExtKey, keyLayout );
   }

   KeyNames = 0;

   if( CareVk != VkCode )
      ScanCode = VkToScan( VkCode, ExtKey, keyLayout );

   if( ExtKey )
      KeyNames = keyLayout->pKeyNamesExt;
   else
      KeyNames = keyLayout->pKeyNames;

   //FIXME!
   //for( i = 0; KeyNames[i].pwsz; i++ )
   //{
   //   if( KeyNames[i].vsc == ScanCode )
   //   {
   //      UINT StrLen = wcslen(KeyNames[i].pwsz);
   //      UINT StrMax = StrLen > (nSize - 1) ? (nSize - 1) : StrLen;
   //      WCHAR null_wc = 0;
   //      if( NT_SUCCESS( MmCopyToCaller( lpString,
   //                                      KeyNames[i].pwsz,
   //                                      StrMax * sizeof(WCHAR) ) ) &&
   //            NT_SUCCESS( MmCopyToCaller( lpString + StrMax,
   //                                        &null_wc,
   //                                        sizeof( WCHAR ) ) ) )
   //      {
   //         ret = StrMax;
   //         break;
   //      }
   //   }
   //}

   //if( ret == 0 )
   //{
   //   WCHAR UCName[2];

   //   UCName[0] = W32kSimpleToupper(IntMapVirtualKeyEx( VkCode, 2, keyLayout ));
   //   UCName[1] = 0;
   //   ret = 1;

   //   if( !NT_SUCCESS(MmCopyToCaller( lpString, UCName, 2 * sizeof(WCHAR) )) )
   //      ret = 0;
   //}

   UNREFERENCED_LOCAL_VARIABLE(KeyNames);

cleanup:
   //UserLeave();
   return ret;
}

/*
    Based on TryToTranslateChar, instead of processing VirtualKey match,
    look for wChar match.
 */
DWORD
APIENTRY
RosUserVkKeyScanEx(
   WCHAR wChar,
   HKL hKeyboardLayout,
   BOOL UsehKL ) // TRUE from KeyboardLayout, FALSE from pkbl = (THREADINFO)->KeyboardLayout
{
   PKBDTABLES KeyLayout;
   PVK_TO_WCHAR_TABLE vtwTbl;
   PVK_TO_WCHARS10 vkPtr;
   size_t size_this_entry;
   int nMod;
   PKBL pkbl = NULL;
   DWORD CapsMod = 0, CapsState = 0, Ret = -1;

   //UserEnterShared();

   if (UsehKL)
   {
      if ( !hKeyboardLayout || !(pkbl = UserHklToKbl(hKeyboardLayout)))
      goto Exit;
   }
   else // From VkKeyScanAW it is FALSE so KeyboardLayout is white noise.
   {
     pkbl = ((PTHREADINFO)PsGetCurrentThreadWin32Thread())->KeyboardLayout;
   }   

   KeyLayout = pkbl->KBTables;

   for (nMod = 0; KeyLayout->pVkToWcharTable[nMod].nModifications; nMod++)
   {
      vtwTbl = &KeyLayout->pVkToWcharTable[nMod];
      size_this_entry = vtwTbl->cbSize;
      vkPtr = (PVK_TO_WCHARS10)((BYTE *)vtwTbl->pVkToWchars);

      while(vkPtr->VirtualKey)
      {
         /*
            0x01 Shift key
            0x02 Ctrl key
            0x04 Alt key
            Should have only 8 valid possibilities. Including zero.
          */
         for(CapsState = 0; CapsState < vtwTbl->nModifications; CapsState++)
         {
            if(vkPtr->wch[CapsState] == wChar)
            {
               CapsMod = KeyLayout->pCharModifiers->ModNumber[CapsState];
               DPRINT("nMod %d wC %04x: CapsMod %08x CapsState %08x MaxModBits %08x\n",
                      nMod, wChar, CapsMod, CapsState, KeyLayout->pCharModifiers->wMaxModBits);
               Ret = ((CapsMod << 8)|(vkPtr->VirtualKey & 0xff));
               goto Exit;
            }
         }
         vkPtr = (PVK_TO_WCHARS10)(((BYTE *)vkPtr) + size_this_entry);
      }
   }
Exit:
   //UserLeave();
   return Ret;
}

BOOL
APIENTRY
RosUserSetAsyncKeyboardState(BYTE key_state_table[])
{
    NTSTATUS Status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        ProbeForRead(key_state_table,sizeof(gQueueKeyStateTable),1);
        RtlCopyMemory(gQueueKeyStateTable,key_state_table,sizeof(gQueueKeyStateTable));
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        Status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;

    return NT_SUCCESS(Status);
}

BOOL
APIENTRY
RosUserGetAsyncKeyboardState(BYTE key_state_table[])
{
    NTSTATUS Status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        ProbeForWrite(key_state_table,sizeof(gQueueKeyStateTable),1); 
        RtlCopyMemory(key_state_table,gQueueKeyStateTable,sizeof(gQueueKeyStateTable));
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        Status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;

    return NT_SUCCESS(Status);
}


/* EOF */