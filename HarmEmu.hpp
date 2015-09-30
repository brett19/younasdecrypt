#ifndef HarmEmu_h__
#define HarmEmu_h__

#include <string>
#include "HarmModuleMgr.hpp"
#include "HarmEmuCore.hpp"
#include "HarmEmuBinaryHash.hpp"
#include "HarmEmuNetwork.hpp"
#include "HarmEmuMisc.hpp"
#include "HarmEmuPacketCrypt.hpp"
#include "HarmEmuRagFile.hpp"
#include "HarmEmuHarmConfig.hpp"
#include "HarmEmuMacAddress.hpp"
#include "HarmEmuTimer.hpp"
#include "HarmEmuNetModule.hpp"

namespace HarmEmu
{
	static HarmModule* LoadMod_MacAddress( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "macaddress.dll" );
		pModule->AddFunction( "?Determine@MacAddress@@YA_NPAE@Z", &HarmEmu::MacAddress::Determine );
		pModule->AddFunction( "?DetermineRemote@MacAddress@@YA_NPAE@Z", &HarmEmu::MacAddress::DetermineRemote );
		pModule->AddFunction( "?PrintMacAddress@MacAddress@@YAXPAE@Z", &HarmEmu::MacAddress::PrintMacAddress );
		return pModule;
	}

	static HarmModule* LoadMod_HarmConfig( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "harmconfig.dll" );
		pModule->AddFunction( "?ReadConfigBool@HarmConfig@@YA_NPBD_N@Z", &HarmEmu::HarmConfig::ReadConfigBool );
		return pModule;
	}

	static HarmModule* LoadMod_RagFile( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "ragfile.dll" );
		pModule->AddFunction( "??0RagFile@@QAE@XZ", &HarmEmu::RagFile::Constructor );
		pModule->AddFunction( "??1RagFile@@QAE@XZ", &HarmEmu::RagFile::Destructor );
		pModule->AddFunction( "?Open@RagFile@@QAE_NPADI_N@Z", &HarmEmu::RagFile::Impl::Open );
		pModule->AddFunction( "?ptr@RagFile@@QAEPAXJ@Z", &HarmEmu::RagFile::Impl::ptr );
		pModule->AddFunction( "?GetLength@RagFile@@QAEKXZ", &HarmEmu::RagFile::Impl::GetLength );
		return pModule;
	}

	static HarmModule* LoadMod_Network( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "network.dll" );
		pModule->AddFunction( "?RegisterStreamConnectHandler@Network@@YAXP6I_NPAUServerAddress@1@@ZH@Z", &HarmEmu::Network::RegisterStreamConnectHandler );
		pModule->AddFunction( "?RegisterStreamConnectAfterHandler@Network@@YAXP6I_NPAUServerAddress@1@@ZH@Z", &HarmEmu::Network::RegisterStreamConnectAfterHandler );
		pModule->AddFunction( "?RegisterStreamDataHandler@Network@@YAXP6IX_NPAEI@ZH@Z", &HarmEmu::Network::RegisterStreamDataHandler );
		pModule->AddFunction( "?RegisterPacket@Network@@YAXGP6I_N_NGPAEI@ZH@Z", &HarmEmu::Network::RegisterPacket );
		pModule->AddFunction( "?SendPacketNoStreamHandler@Network@@YAXPAEI@Z", &HarmEmu::Network::SendPacketNoStreamHandler );
		pModule->AddFunction( "?RegisterPacketChainHandler@Network@@YAXP6I_N_NGPAEIH@ZH@Z", &HarmEmu::Network::RegisterPacketChainHandler );
		pModule->AddFunction( "?IsZoneConnection@Network@@YA_NXZ", &HarmEmu::Network::IsZoneConnection );
		pModule->AddFunction( "?SendPacket@Network@@YAXPAEI_N@Z", &HarmEmu::Network::SendPacket );
		pModule->AddFunction( "?EnterStealthMode@Network@@YAXXZ", &HarmEmu::Network::EnterStealthMode );
		pModule->AddFunction( "?SetPacketSize@Network@@YAXHH@Z", &HarmEmu::Network::SetPacketSize );
		pModule->AddFunction( "?GetPacketSize@Network@@YAHH@Z", &HarmEmu::Network::GetPacketSize );
		pModule->AddFunction( "?RegisterPacketIDTranslator@Network@@YAXP6I_N_NPAGPA_N@ZH@Z", &HarmEmu::Network::RegisterPacketIDTranslator );
		pModule->AddFunction( "?SetPacketHandler@Network@@YAXHPAX@Z", &HarmEmu::Network::SetPacketHandler );
		pModule->AddFunction( "?GetPacketHandler@Network@@YAPAXH_N@Z", &HarmEmu::Network::GetPacketHandler );
		return pModule;
	}

	static HarmModule* LoadMod_CpuId( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "cpuid.dll" );
		pModule->AddFunction( "?cpu_VendorID@CPU@@3PADA", &HarmEmu::CpuId::__Generic );
		pModule->AddFunction( "?cpu_branding@CPU@@3PADA", &HarmEmu::CpuId::__Generic );
		return pModule;
	}

	static HarmModule* LoadMod_GrfLib( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "grflib.dll" );
		pModule->AddFunction( "?RegisterOpenCallback@GrfLib@@YA_NPBDP6AX0PAXI1@Z1@Z", &HarmEmu::GrfLib::__Generic );
		return pModule;
	}

	static HarmModule* LoadMod_License( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "license.dll" );
		pModule->AddFunction( "?isActive@RestrictedMode@License@@YA_NXZ", &HarmEmu::License::__Generic );
		return pModule;
	}

	static HarmModule* LoadMod_Timer( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "timer.dll" );
		pModule->AddFunction( "?Add@Timer@@YAHIP6AXPAUTimerData@1@@ZHH_JPAX@Z", &HarmEmu::Timer::Add );
		pModule->AddFunction( "?AddInterval@Timer@@YAHIIP6AXPAUTimerData@1@@ZHH_JPAX@Z", &HarmEmu::Timer::AddInterval );
		pModule->AddFunction( "?Get@Tick@@YAIXZ", &HarmEmu::Tick::Get );
		return pModule;
	}
	
	static HarmModule* LoadMod_NetModule( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "netmodule.dll" );
		pModule->AddFunction( "?GetBufferData@Task@NetModule@@QAEXPAPAXPAI@Z", &HarmEmu::NetModule::__Generic );
		pModule->AddFunction( "?RegisterTaskCompletionCallback@NetModule@@YAXP6IXPAVTask@1@@ZH@Z", &HarmEmu::NetModule::RegisterTaskCompletionCallback );
		pModule->AddFunction( "?IsSuccess@Task@NetModule@@QAE_NXZ", &HarmEmu::NetModule::__Generic );
		return pModule;
	}

	static HarmModule* LoadMod_ToolBlock( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "toolblock.dll" );
		pModule->AddFunction( "?EnableAutocheck@CodeModification@ToolBlock@@YAXI@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?ProtectMemory@MemoryProtect@ToolBlock@@YA_NXZ", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterForbiddenLibrary@Dll@ToolBlock@@YAXIH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?EnableAutocheck@PacketEditor@ToolBlock@@YAXI@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?EnableDetection@SpeedHack@ToolBlock@@YAX_N@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterDetectFunc@PacketEditor@ToolBlock@@YAXP6IXXZH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterDetectFunc@SpeedHack@ToolBlock@@YAXP6IXXZH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?EnableAutocheck@Debugger@ToolBlock@@YAXI@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?ProtectionFailFlags@MemoryProtect@ToolBlock@@YAHXZ", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterDebuggerCallback@Debugger@ToolBlock@@YAXP6IXXZH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterCodeModificationCallback@CodeModification@ToolBlock@@YAXP6IXXZH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?GetMismatchData@CodeModification@ToolBlock@@YAPAUModByte@MemTrace@@PAI@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?RegisterUnprotectedFunc@MemoryProtect@ToolBlock@@YAXP6IXXZH@Z", &HarmEmu::ToolBlock::__Generic );
		pModule->AddFunction( "?EnableAutocheck@MemoryProtect@ToolBlock@@YAXI@Z", &HarmEmu::ToolBlock::__Generic );
		return pModule;
	}

	static HarmModule* LoadMod_BinaryHash( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "binaryhash.dll" );
		pModule->AddFunction( "??1MD5@@QAE@XZ", &HarmEmu::BinaryHash::__Generic );
		pModule->AddFunction( "?Final@MD5@@QAEXPAE@Z", &HarmEmu::BinaryHash::__Generic );
		pModule->AddFunction( "??0MD5@@QAE@XZ", &HarmEmu::BinaryHash::__Generic );
		pModule->AddFunction( "?Update@MD5@@QAEXPAEI@Z", &HarmEmu::BinaryHash::__Generic );
		pModule->AddFunction( "?RoBinaryHash@BinaryHash@@YA_NPAE@Z", &HarmEmu::BinaryHash::RoBinaryHash );
		return pModule;
	}

	static HarmModule* LoadMod_Core( HarmModuleMgr* pManager )
	{
		HarmModuleVirt* pModule = pManager->CreateVirtModule( "core.dll" );
		pModule->AddFunction( 0x59fb02f0, &HarmEmu::Core::InitModule );
		pModule->AddFunction( 0xf2d1abb8, &HarmEmu::Core::MemAlloc );
		pModule->AddFunction( 0xc910e53d, &HarmEmu::Core::MemFree );
		pModule->AddFunction( 0x8c817fdd, &HarmEmu::Core::SeedRand );
		pModule->AddFunction( 0x9cf8d181, &HarmEmu::Core::Rand );
		pModule->AddFunction( 0xd608a7a5, &HarmEmu::Core::AES::Constructor );
		pModule->AddFunction( 0xa0f5f9a1, &HarmEmu::Core::AES::Impl::SetParameters );
		pModule->AddFunction( 0xdd722478, &HarmEmu::Core::AES::Impl::StartEncryption );
		pModule->AddFunction( 0x96f27e11, &HarmEmu::Core::AES::Impl::Encrypt );
		pModule->AddFunction( 0xafabd35e, &HarmEmu::Core::CalcCrc32 );
		pModule->AddFunction( 0x4b5d70ec, &HarmEmu::Core::func_4b5d70ec );
		pModule->AddFunction( 0x8ad8e3fe, &HarmEmu::Core::func_8ad8e3fe );
		return pModule;
	}

	HarmModule* LoadModule( HarmModuleMgr* pManager, const std::string& sModuleName )
	{
		if( sModuleName == "core.dll" ) {
			return LoadMod_Core( pManager );
		} else if( sModuleName == "binaryhash.dll" ) {
			return LoadMod_BinaryHash( pManager );
		} else if( sModuleName == "toolblock.dll" ) {
			return LoadMod_ToolBlock( pManager );
		} else if( sModuleName == "license.dll" ) {
			return LoadMod_License( pManager );
		} else if( sModuleName == "timer.dll" ) {
			return LoadMod_Timer( pManager );
		} else if( sModuleName == "netmodule.dll" ) {
			return LoadMod_NetModule( pManager );
		} else if( sModuleName == "grflib.dll" ) {
			return LoadMod_GrfLib( pManager );
		} else if( sModuleName == "cpuid.dll" ) {
			return LoadMod_CpuId( pManager );
		} else if( sModuleName == "macaddress.dll" ) {
			return LoadMod_MacAddress( pManager );
		} else if( sModuleName == "network.dll" ) {
			return LoadMod_Network( pManager );
		} else if( sModuleName == "ragfile.dll" ) {
			return LoadMod_RagFile( pManager );
		} else if( sModuleName == "harmconfig.dll" ) {
			return LoadMod_HarmConfig( pManager );
		} else if( sModuleName == "packetcrypt.dll" ) {
			throw new std::exception( "Only helper functions are available in this module." );
		} else {
			throw new std::exception( "Attempted to load invalid HarmEmu module." );
		}
		return nullptr;
	}
}

#endif // HarmEmu_h__
