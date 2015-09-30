#ifndef HarmCtrl_h__
#define HarmCtrl_h__

#include "HarmModuleBundle.hpp"
#include "HarmModuleReal.hpp"
#include "HarmModuleVirt.hpp"
#include "HarmModuleWin32.hpp"
#include "HarmModuleMgr.hpp"
#include "HarmEmu.hpp"

namespace HarmCtrl
{
	HarmModuleMgr xHarmMgr;

	unsigned int __stdcall H_GetTickCount( )
	{
		return GetTickCount( );
	}

	BOOL WINAPI H_GetUserNameA( __out LPTSTR lpBuffer, __inout LPDWORD lpnSize )
	{
		strcpy( lpBuffer, "Administrator" );
		*lpnSize = strlen(lpBuffer) + 1;
		return TRUE;
	}

	BOOL WINAPI H_GetComputerNameA( __out LPTSTR lpBuffer, __inout LPDWORD lpnSize )
	{
		strcpy( lpBuffer, "FutureShop-PC" );
		*lpnSize = strlen(lpBuffer) + 1;
		return TRUE;
	}

	void LoadHarmModules( )
	{
		HarmModuleWin32* pModUser32 = xHarmMgr.LoadWin32Module( "user32.dll" );
		HarmModuleWin32* pModNtdll = xHarmMgr.LoadWin32Module( "ntdll.dll" );
		HarmModuleWin32* pModKernel32 = xHarmMgr.LoadWin32Module( "kernel32.dll" );
		HarmModuleWin32* pModAdvApi32 = xHarmMgr.LoadWin32Module( "advapi32.dll" );

		HarmModuleBundle xHarmBndl;
		xHarmBndl.Load( "D:\\Apps\\Ragray\\harmony\\younas.pkg" );
		HarmModuleReal* pModIdCrypt = xHarmMgr.LoadBundleModule( &xHarmBndl, "idcrypt.dll" );
		HarmModuleReal* pModPakCrypt = xHarmMgr.LoadBundleModule( &xHarmBndl, "packetcrypt.dll" );
		HarmModuleReal* pModIdScramble = xHarmMgr.LoadBundleModule( &xHarmBndl, "idscramble.dll" );
		HarmModuleReal* pModCustomer = xHarmMgr.LoadBundleModule( &xHarmBndl, "customer.dll" );

		HarmEmu::LoadModule( &xHarmMgr, "toolblock.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "binaryhash.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "grflib.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "netmodule.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "license.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "timer.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "cpuid.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "network.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "ragfile.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "harmconfig.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "macaddress.dll" );
		HarmEmu::LoadModule( &xHarmMgr, "core.dll" );

		HarmEmu::Core::Init( &xHarmMgr );

		pModKernel32->AddHookFunction( "GetTickCount", &H_GetTickCount );
		pModKernel32->AddHookFunction( "GetComputerNameA", &H_GetComputerNameA );
		pModAdvApi32->AddHookFunction( "GetUserNameA", &H_GetUserNameA );
		//pModPakCrypt->SetBreakpoint( "?Enable@PacketCrypt@@YAXPAE@Z" );

		xHarmMgr.LinkAll( );
		xHarmMgr.EnterAll( );
	}

	void SetHarmStage( int iStage )
	{
		typedef void (__cdecl *F_onHarmStage)( int iStage );

		HarmModule* xModCustomer = xHarmMgr.GetModule( "customer.dll" );
		HarmModule* xModPakCrypt = xHarmMgr.GetModule( "packetcrypt.dll" );

		F_onHarmStage Func_Customer_onHarmStage = (F_onHarmStage)xModCustomer->GetProcAddr( "?onHarmStage@@YAXH@Z" );
		F_onHarmStage Func_PakCrypt_onHarmStage = (F_onHarmStage)xModPakCrypt->GetProcAddr( "?onHarmStage@@YAXH@Z" );

		Func_PakCrypt_onHarmStage( iStage );
		Func_Customer_onHarmStage( iStage );
	}


	void LoadHarmony( )
	{
		printf( "--------------------- PREPARING HARMONY MODULES ---------------------\n" );
		LoadHarmModules( );

		printf( "---------------------    EMULATING GAME INIT    ---------------------\n" );
		SetHarmStage( 0 );
		SetHarmStage( 1 );
		SetHarmStage( 3 );
		SetHarmStage( 4 );
		SetHarmStage( 5 );
		SetHarmStage( 6 );
		SetHarmStage( 7 );

		printf( "--------------------- COMPLETED HARMONY LOADING ---------------------\n" );
	}

}

#endif // HarmCtrl_h__
