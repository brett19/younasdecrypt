#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <winnt.h>
#include "harmony.h"
#include "crc32.h"

#include "HarmModuleBundle.hpp"




	/*
	RoPakLogin xTest;
	memset( &xTest, 0x00, sizeof(xTest) );
	xTest.cmd = 0x64;
	xTest.server_ver = 0x0132B4E4;
	xTest.server_type = 0x0E;
	strcpy( xTest.username, "belivious" );
	strcpy( xTest.password, "diablo" );

	HarmEmu::Network::ServerAddress xAddr;
	xAddr.uiAddr = 0;
	strcpy( xAddr.pcAddr, "208.43.76.98" );
	xAddr.uiPort = 6984;

	HarmEmu::Network::F_PacketHandler fHandler = HarmEmu::Network::g_rPacketHandlers[ 0x64 ];
	bool bRetval = fHandler( true, 0x64, (unsigned char*)&xTest, sizeof(xTest) );
	printf( "PacketHandler returned %s\n", bRetval?"true":"false" );
	*/
	/*
	unsigned char peer_key[32];
	unsigned char peer0_0[] = {
		0x46, 0x65, 0xe0, 0x5d, 0xcd, 0xb8, 0x76, 0x2e,
		0x9c, 0xaf, 0x1e, 0x41, 0xe8, 0x31, 0x42, 0xce, 0x15, 0x05, 0x23, 0x24, 0x96, 0xa9, 0x32, 0x66,
		0x6b, 0x86, 0x01, 0x8b, 0xdb, 0x9f, 0x08, 0xfc, 0xce, 0xf5, 0x0d, 0xd7, 0x4a, 0x6e, 0x6a, 0xc4,
		0xbc, 0x77, 0xbb, 0x5c, 0x09, 0x59, 0x6b, 0x62, 0x06, 0x42, 0x04, 0x43, 0xbc, 0x84, 0xa9, 0x15,
		0xdf, 0x8a, 0x6a, 0x91, 0x77, 0x83, 0x6e, 0xdf, 0x69, 0x5d, 0xe4, 0xb1, 0x9b, 0xaa, 0x62, 0xc9,
		0xbb, 0xc8, 0xca, 0xdc, 0xec, 0x48, 0xd9, 0xa4, 0xba, 0x01, 0xa5, 0x0f, 0xc5, 0xb0, 0x40, 0x60
	};
	unsigned char peer_decrypt[ 5 * 16 ];

	HarmEmu::PacketCrypt::CalcKey( peer_key, peer0_0 );

	AES crypt;
	crypt.SetParameters( 256 );
	crypt.StartDecryption( peer_key );
	crypt.Decrypt( &peer0_0[8], peer_decrypt, 5, AES::CBC );
	PrintBinary( peer_decrypt, 5 * 16 );
	*/



class LookupTable
{
public:
	void Insert( unsigned int sKey, const std::string& sText ) 
	{
		mList.insert( LookupValue( sKey, sText ) );
	}

	void Insert( const std::string& sKey, const std::string& sText )
	{
		Insert( GetCrc32(sKey), sText );
	}

	void Insert( const std::string& sText )
	{
		Insert( sText, sText );
	}

	std::string Get( unsigned int sValue  )
	{
		LookupList::iterator i = mList.find(sValue);
		if( i != mList.end( ) ) {
			return (*i).second;
		}
		sprintf( sTemp, "*UNKNOWN*", sValue );
		return sTemp;
	}

protected:
	typedef std::pair< unsigned int, std::string > LookupValue;
	typedef std::unordered_map< unsigned int, std::string > LookupList;

	LookupList mList;
	char sTemp[ 256 ];

};

LookupTable mModules;
LookupTable mFunctions;

void ReadModuleFuncs( const std::string& sModuleName )
{
	HMODULE lib = GetModuleHandle( sModuleName.c_str() );
	if( !lib ) {
		printf( "Loading Library %s\n", sModuleName.c_str() );
		lib = LoadLibrary(sModuleName.c_str());
	}
	if( !lib ) { return; }
	if(((PIMAGE_DOS_HEADER)lib)->e_magic != IMAGE_DOS_SIGNATURE) {
		return;
	}
	PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)( (BYTE*)lib + ((PIMAGE_DOS_HEADER)lib)->e_lfanew );
	if(header->Signature != IMAGE_NT_SIGNATURE) { return; }
	if(header->OptionalHeader.NumberOfRvaAndSizes <= 0) { return; }
	PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)( (BYTE*)lib + header->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );
	PVOID names = (BYTE *)lib + exports->AddressOfNames;
	for (int i = 0; i < exports->NumberOfNames; i++) {
		char* exportname = (char*)( (BYTE *)lib + ((DWORD *)names)[i] );
		mFunctions.Insert( exportname );
	}
}

void AddModule( const std::string& sModuleName )
{
	mModules.Insert( sModuleName );
	ReadModuleFuncs( sModuleName );
}

int main( int argc, char* argv[] )
{
	InitCrc32( );

	// Windows Modules
	AddModule( "kernel32.dll" );
	AddModule( "advapi32.dll" );
	AddModule( "user32.dll" );
	AddModule( "ntdll.dll" );
	AddModule( "mswsock.dll" );
	AddModule( "ws2_32.dll" );
	AddModule( "shell32.dll" );

	// LRO Modules
	AddModule( "lro.exe" );
	AddModule( "granny2.dll" );
	AddModule( "npklib.dll" );
	AddModule( "binkw32.dll" );
	
	// Harmony Modules
	AddModule( "core.dll" );

	AddModule( "harmconfig.dll" );
	AddModule( "ragfile.dll" );
	AddModule( "cpuid.dll" );
	AddModule( "macaddress.dll" );
	AddModule( "modtrace.dll" );
	AddModule( "base64.dll" );
	AddModule( "db.dll" );
	AddModule( "memtrace.dll" );
	AddModule( "hook.dll" );
	AddModule( "crashhandle.dll" );
	AddModule( "timer.dll" );
	AddModule( "fileprotect.dll" );
	AddModule( "splash.dll" );
	AddModule( "screenshot.dll" );
	AddModule( "netmodule.dll" );
	AddModule( "network.dll" );
	AddModule( "binaryhash.dll" );
	AddModule( "license.dll" );
	AddModule( "idcrypt.dll" );
	AddModule( "grflib.dll" );
	AddModule( "packetcrypt.dll" );
	AddModule( "idscramble.dll" );
	AddModule( "3rdpartynet.dll" );
	AddModule( "toolblock.dll" );
	AddModule( "threaddetect.dll" );
	AddModule( "customer.dll" );

	// Exported functions

	mFunctions.Insert( 0x59fb02f0, "***InitHarmModule" );
	mFunctions.Insert( "?onHarmStage@@YAXH@Z", "onHarmStage" );						// void onHarmStage( int )
	mFunctions.Insert( "Console@@QAE@XZ" );
	mFunctions.Insert( "??0Console@@QAE@XZ" );
	mFunctions.Insert( "??0SHA1@@QAE@XZ" );
	mFunctions.Insert( "?ReadConfigBool@HarmConfig@@YA_NPBD_N@Z" );
	mFunctions.Insert( "?ReadConfigInt@HarmConfig@@YAHPBDH@Z" );
	mFunctions.Insert( "??0RagFile@@QAE@PAURagFileI@@@Z" );
	mFunctions.Insert( "??0RagFile@@QAE@XZ" );
	mFunctions.Insert( "??1RagFile@@QAE@XZ" );
	mFunctions.Insert( "??4RagFile@@QAEAAV0@ABV0@@Z" );
	mFunctions.Insert( "?Close@RagFile@@QAEXXZ" );
	mFunctions.Insert( "?DebugOutput@RagFile@@SAX_N@Z" );
	mFunctions.Insert( "?GetHandle@RagFile@@QAEPAURagFileI@@XZ" );
	mFunctions.Insert( "?GetLength@RagFile@@QAEKXZ" );
	mFunctions.Insert( "?Open@RagFile@@QAE_NPADI_N@Z" );
	mFunctions.Insert( "?Read@RagFile@@QAE_NPAXI@Z" );
	mFunctions.Insert( "?Seek@RagFile@@QAE_NJI@Z" );
	mFunctions.Insert( "?Write@RagFile@@QAE_NPAXI@Z" );
	mFunctions.Insert( "?ptr@RagFile@@QAEPAXJ@Z" );
	mFunctions.Insert( "?cpu_3dnow@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_VendorID@CPU@@3PADA" );
	mFunctions.Insert( "?cpu_branding@CPU@@3PADA" );
	mFunctions.Insert( "?cpu_cmov@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_fcmov@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_mmx@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_rdtsc@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_sse2@CPU@@3_NA" );
	mFunctions.Insert( "?cpu_sse@CPU@@3_NA" );
	mFunctions.Insert( "?cpuid@CPU@@YA_NKPAK000@Z" );
	mFunctions.Insert( "?Determine1@MacAddress@@YA_NPAE0@Z" );
	mFunctions.Insert( "?Determine2@MacAddress@@YA_NPAE@Z" );
	mFunctions.Insert( "?Determine@MacAddress@@YA_NPAE@Z" );
	mFunctions.Insert( "?DetermineByDevice@MacAddress@@YA_NPAE@Z" );
	mFunctions.Insert( "?DetermineRemote@MacAddress@@YA_NPAE@Z" );
	mFunctions.Insert( "?GetLastError@MacAddress@@YAHXZ" );
	mFunctions.Insert( "?IsBad@MacAddress@@YA_NPAE@Z" );
	mFunctions.Insert( "?PrintMacAddress@MacAddress@@YAXPAE@Z" );
	mFunctions.Insert( "?flags@?1??Determine2@MacAddress@@YA_NPAE@Z@4KB" );
	mFunctions.Insert( "?GetPtrEntry@ModTrace@@YAPAUTraceEntry@1@PAX@Z" );
	mFunctions.Insert( "?GetPtrType@ModTrace@@YA?AW4TraceType@1@PAX@Z" );
	mFunctions.Insert( "?Decode@Base64@@YAIPADIPAE@Z" );
	mFunctions.Insert( "?Encode@Base64@@YAIPAEIPAD@Z" );
	mFunctions.Insert( "??0DB@@QAE@PBDI@Z" );
	mFunctions.Insert( "??4DB@@QAEAAV0@ABV0@@Z" );
	mFunctions.Insert( "??_FDB@@QAEXXZ?Delete@DB@@QAEPAX_K@Z" );
	mFunctions.Insert( "?DeleteStr@DB@@QAEPAXPBDI@Z" );
	mFunctions.Insert( "?Fetch@DB@@QAEPAX_K@Z" );
	mFunctions.Insert( "?FetchStr@DB@@QAEPAXPBDI@Z" );
	mFunctions.Insert( "?Final@DB@@QAEXP6AX_KPAX@Z@Z" );
	mFunctions.Insert( "?ForEach@DB@@QAEXP6AH_KPAX@Z@Z" );
	mFunctions.Insert( "?ForEachVA@DB@@QAAXP6AH_KPAXPAE@ZZZ" );
	mFunctions.Insert( "?Insert@DB@@QAEX_KPAX@Z" );
	mFunctions.Insert( "?InsertStr@DB@@QAEXPBDIPAX@Z" );
	mFunctions.Insert( "?Size@DB@@QAEIXZ" );
	mFunctions.Insert( "?db_free_lock@DB@@AAEXXZ" );
	mFunctions.Insert( "?db_free_unlock@DB@@AAEXXZ" );
	mFunctions.Insert( "?db_rebalance@DB@@CAXPAUDBNS@@PAPAU2@@Z" );
	mFunctions.Insert( "?db_rebalance_erase@DB@@CAXPAUDBNS@@PAPAU2@@Z" );
	mFunctions.Insert( "?db_rotate_left@DB@@CAXPAUDBNS@@PAPAU2@@Z" );
	mFunctions.Insert( "?db_rotate_right@DB@@CAXPAUDBNS@@PAPAU2@@Z" );
	mFunctions.Insert( "?dbnpool_alloc@DB@@AAEXPAV1@I@Z" );
	mFunctions.Insert( "?dbnpool_final@DB@@AAEXXZ" );
	mFunctions.Insert( "?dbnpool_get@DB@@AAEPAUDBNS@@XZ" );
	mFunctions.Insert( "?dbnpool_put@DB@@AAEXPAUDBNS@@@Z" );
	mFunctions.Insert( "?DetectMismatches@MemTrace@@YAPAUModByte@1@PAI@Z" );
	mFunctions.Insert( "?PatchByte@MemTrace@@YAXPAXE@Z" );
	mFunctions.Insert( "?PatchDword@MemTrace@@YAXPAXI@Z" );
	mFunctions.Insert( "?PatchWord@MemTrace@@YAXPAXG@Z" );
	mFunctions.Insert( "?RegisterTraceArea@MemTrace@@YAXPAXI@Z" );
	mFunctions.Insert( "??0VirtualProtection@@QAE@PAXKK@Z" );
	mFunctions.Insert( "??1VirtualProtection@@QAE@XZ" );
	mFunctions.Insert( "??4VirtualProtection@@QAEAAV0@ABV0@@Z" );
	mFunctions.Insert( "?DetourFunction@Hook@@YAPAXPAX0@Z" );
	mFunctions.Insert( "?PlaceDetour@Hook@@YAXPAX0_N@Z" );
	mFunctions.Insert( "?ok@VirtualProtection@@QAE_NXZ" );
	mFunctions.Insert( "?CrashHandlerBusy@CrashHandle@@3_NC" );
	mFunctions.Insert( "?CrashTime@CrashHandle@@3IA" );
	mFunctions.Insert( "?RegisterCrashDumpInfo@CrashHandle@@YAXP6AXXZ@Z" );
	mFunctions.Insert( "?WriteCrashLog@CrashHandle@@YAXPBDZZ" );
	mFunctions.Insert( "??0ProcessTimer@@QAE@XZ" );
	mFunctions.Insert( "??4ProcessTimer@@QAEAAV0@ABV0@@Z" );
	mFunctions.Insert( "?Add@Timer@@YAHIP6AXPAUTimerData@1@@ZHH_JPAX@Z" );
	mFunctions.Insert( "?AddInterval@Timer@@YAHIIP6AXPAUTimerData@1@@ZHH_JPAX@Z" );
	mFunctions.Insert( "?Continue@ProcessTimer@@QAEXXZ" );
	mFunctions.Insert( "?Delete@Timer@@YAHH@Z" );
	mFunctions.Insert( "?DeleteFuncCompare@Timer@@YAHHP6AXPAUTimerData@1@@Z@Z" );
	mFunctions.Insert( "?Get@ProcessTimer@@QAEIXZ" );
	mFunctions.Insert( "?Get@Tick@@YAIXZ" );
	mFunctions.Insert( "?Get@Timer@@YAPAUTimerData@1@H@Z" );
	mFunctions.Insert( "?Pause@ProcessTimer@@QAEXXZ" );
	mFunctions.Insert( "?Reset@ProcessTimer@@QAEXXZ" );
	mFunctions.Insert( "?onHarmStage@@YAXH@Z" );
	mFunctions.Insert( "?ProcessFileProtect@FileProtect@@YA_NAAPAXAAI@Z" );
	mFunctions.Insert( "??0Task@NetModule@@QAE@XZ??1Task@NetModule@@QAE@XZ" );
	mFunctions.Insert( "??4Task@NetModule@@QAEAAV01@ABV01@@Z" );
	mFunctions.Insert( "?CancelHttpRequest@Task@NetModule@@AAEXXZ" );
	mFunctions.Insert( "?GetBufferData@Task@NetModule@@QAEXPAPAXPAI@Z" );
	mFunctions.Insert( "?IsCompleted@Task@NetModule@@QAE_NPA_N@Z" );
	mFunctions.Insert( "?IsSuccess@Task@NetModule@@QAE_NXZ" );
	mFunctions.Insert( "?PrepareHttpRequest@Task@NetModule@@AAE_NXZ" );
	mFunctions.Insert( "?PrepareRequest@Task@NetModule@@QAE_NPBD@Z" );
	mFunctions.Insert( "?RegisterTaskCompletionCallback@NetModule@@YAXP6IXPAVTask@1@@ZH@Z" );
	mFunctions.Insert( "?SendHttpRequest@Task@NetModule@@AAE_NXZ" );
	mFunctions.Insert( "?SendHttpRequestExecute@Task@NetModule@@AAE_NXZ" );
	mFunctions.Insert( "?SendRequest@Task@NetModule@@QAE_NPAPAXPAI_N@Z" );
	mFunctions.Insert( "?ThreadProc@Task@NetModule@@CGKPAX@Z" );
	mFunctions.Insert( "?default_max_buffer_size@Task@NetModule@@0IB" );

	mFunctions.Insert( "??4PacketQueue@Network@@QAEAAV01@ABV01@@Z" );
	mFunctions.Insert( "?DiscardPacketProc@Network@@YI_N_NGPAEI@Z" );
	mFunctions.Insert( "?EnterStealthMode@Network@@YAXXZ" );
	mFunctions.Insert( "?GetData@PacketQueue@Network@@QAE_NHPAE@Z" );
	mFunctions.Insert( "?GetPacketHandler@Network@@YAPAXH_N@Z" );
	mFunctions.Insert( "?GetPacketID@Network@@YAG_NPAEI@Z" );
	mFunctions.Insert( "?GetPacketSize@Network@@YAHH@Z" );
	mFunctions.Insert( "?GetRecvQueue@Network@@YAPAVPacketQueue@1@XZ" );
	mFunctions.Insert( "?GetSendQueue@Network@@YAPAVPacketQueue@1@XZ" );
	mFunctions.Insert( "?GetSize@PacketQueue@Network@@QAEHXZ" );
	mFunctions.Insert( "?HandlerRegistered@Network@@YA_NGP6I_N_NGPAEI@Z@Z" );
	mFunctions.Insert( "?InetAddr@Network@@YAIPBD@Z" );
	mFunctions.Insert( "?InsertData@PacketQueue@Network@@QAEXHPBE@Z" );
	mFunctions.Insert( "?IsConnected@Network@@YA_NXZ" );
	mFunctions.Insert( "?IsDiscarded@Network@@YA_NG@Z" );
	mFunctions.Insert( "?IsZoneConnection@Network@@YA_NXZ" );
	mFunctions.Insert( "?PeekData@PacketQueue@Network@@QAE_NHPAE@Z" );
	mFunctions.Insert( "?RegisterPacket@Network@@YAXGP6I_N_NGPAEI@ZH@Z" );
	mFunctions.Insert( "?RegisterPacketChainHandler@Network@@YAXP6I_N_NGPAEIH@ZH@Z" );
	mFunctions.Insert( "?RegisterPacketIDTranslator@Network@@YAXP6I_N_NPAGPA_N@ZH@Z" );
	mFunctions.Insert( "?RegisterStealthModeCallback@Network@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterStreamConnectAfterHandler@Network@@YAXP6I_NPAUServerAddress@1@@ZH@Z" );
	mFunctions.Insert( "?RegisterStreamConnectHandler@Network@@YAXP6I_NPAUServerAddress@1@@ZH@Z" );
	mFunctions.Insert( "?RegisterStreamDataHandler@Network@@YAXP6IX_NPAEI@ZH@Z" );
	mFunctions.Insert( "?RemoveData@PacketQueue@Network@@QAE_NH@Z" );
	mFunctions.Insert( "?SendPacket@Network@@YAXPAEI_N@Z" );
	mFunctions.Insert( "?SendPacketNoStreamHandler@Network@@YAXPAEI@Z" );
	mFunctions.Insert( "?SetPacketHandler@Network@@YAXHPAX@Z" );
	mFunctions.Insert( "?SetPacketSize@Network@@YAXHH@Z" );

	mFunctions.Insert( "??0MD5@@QAE@XZ" );
	mFunctions.Insert( "??1MD5@@QAE@XZ" );
	mFunctions.Insert( "??4MD5@@QAEAAV0@ABV0@@Z" );
	mFunctions.Insert( "?BinaryHash@0@YA_NPADPAE@Z" );
	mFunctions.Insert( "?Final@MD5@@QAEXPAE@Z" );
	mFunctions.Insert( "?GrfHash@BinaryHash@@YA_NPADPAE@Z" );
	mFunctions.Insert( "?GrfHashBlock@BinaryHash@@YA_NHPAE@Z" );
	mFunctions.Insert( "?RegisterGrfHashBlock@BinaryHash@@YA_NPAEI@Z" );
	mFunctions.Insert( "?RoBinaryHash@BinaryHash@@YA_NPAE@Z" );
	mFunctions.Insert( "?Update@MD5@@QAEXPAEI@Z" );
	mFunctions.Insert( "?Enter@RestrictedMode@License@@YAXXZ" );
	mFunctions.Insert( "?Leave@RestrictedMode@License@@YAXXZ" );
	mFunctions.Insert( "?RegisterCallback@RestrictedMode@License@@YAXP6IX_N@ZH@Z" );
	mFunctions.Insert( "?harm_license@License@@3PAUharm_license_t@1@A" );
	mFunctions.Insert( "?isActive@RestrictedMode@License@@YA_NXZ" );
	mFunctions.Insert( "?Init@IDCrypt@@YAXIPBEI@Z" );
	mFunctions.Insert( "?Close@GrfLib@@YAXPAURagFileI@@@Z" );
	mFunctions.Insert( "?Fetch@GrfLib@@YAPAURagFileI@@IPAU2@_N@Z" );
	mFunctions.Insert( "?Fetch@GrfLib@@YAPAURagFileI@@PBDPAU2@_N@Z" );
	mFunctions.Insert( "?RegisterGRF@GrfLib@@YAXPBD@Z" );
	mFunctions.Insert( "?RegisterOpenCallback@GrfLib@@YA_NPBDP6AX0PAXI1@Z1@Z" );
	mFunctions.Insert( "?Enable@PacketCrypt@@YAXPAE@Z", "PacketCrypt::Enable" );	// void Enable( unsigned char* )
	mFunctions.Insert( "?Init@IDScramble@@YAXI_N@Z" );
	mFunctions.Insert( "?TranslateID@IDScramble@@YAGGH@Z" );
	mFunctions.Insert( "?Check@ThirdPartyNet@@YAXPAX0I@Z" );
	mFunctions.Insert( "??0CCheck@Debugger@ToolBlock@@QAE@ABV012@@Z" );
	mFunctions.Insert( "??0CCheck@Debugger@ToolBlock@@QAE@XZ" );
	mFunctions.Insert( "??1CCheck@Debugger@ToolBlock@@QAE@XZ" );
	mFunctions.Insert( "??4CCheck@Debugger@ToolBlock@@QAEAAV012@ABV012@@Z" );
	mFunctions.Insert( "??_7CCheck@Debugger@ToolBlock@@6B@" );
	mFunctions.Insert( "?ApiHooked@ToolBlock@@YAHPAX_K@Z" );
	mFunctions.Insert( "?AutocheckEnabled@CodeModification@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?AutocheckEnabled@MemoryProtect@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?AutocheckEnabled@PacketEditor@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?Check@Debugger@ToolBlock@@YAIXZ" );
	mFunctions.Insert( "?CheckForHooks@PacketEditor@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?Count@PacketEditor@ToolBlock@@YAIXZ" );
	mFunctions.Insert( "?DisableAutocheck@CodeModification@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?DisableAutocheck@Debugger@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?DisableAutocheck@MemoryProtect@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?DisableAutocheck@PacketEditor@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?EnableAutocheck@CodeModification@ToolBlock@@YAXI@Z" );
	mFunctions.Insert( "?EnableAutocheck@Debugger@ToolBlock@@YAXI@Z" );
	mFunctions.Insert( "?EnableAutocheck@MemoryProtect@ToolBlock@@YAXI@Z" );
	mFunctions.Insert( "?EnableAutocheck@PacketEditor@ToolBlock@@YAXI@Z" );
	mFunctions.Insert( "?EnableDetection@SpeedHack@ToolBlock@@YAX_N@Z" );
	mFunctions.Insert( "?GetMismatchData@CodeModification@ToolBlock@@YAPAUModByte@MemTrace@@PAI@Z" );
	mFunctions.Insert( "?GetNotifyDetectionScore@Debugger@ToolBlock@@YAIXZ" );
	mFunctions.Insert( "?IsClean@CodeModification@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?IsDebugged@Debugger@ToolBlock@@YA_N_NI@Z" );
	mFunctions.Insert( "?LastHookRemoved@PacketEditor@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?ProtectMemory@MemoryProtect@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?Protected@MemoryProtect@ToolBlock@@YA_NXZ" );
	mFunctions.Insert( "?ProtectionFailFlags@MemoryProtect@ToolBlock@@YAHXZ" );
	mFunctions.Insert( "?Register@CCheck@Debugger@ToolBlock@@UAEXXZ" );
	mFunctions.Insert( "?RegisterCodeModificationCallback@CodeModification@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterDebugCheck@Debugger@ToolBlock@@YAXPAVCCheck@12@@Z" );
	mFunctions.Insert( "?RegisterDebuggerCallback@Debugger@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterDetectFunc@PacketEditor@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterDetectFunc@SpeedHack@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterForbiddenLibrary@Dll@ToolBlock@@YAXIH@Z" );
	mFunctions.Insert( "?RegisterForbiddenLibrary@Dll@ToolBlock@@YAXPADH@Z" );
	mFunctions.Insert( "?RegisterForbiddenLibraryCallback@Dll@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?RegisterUnprotectedFunc@MemoryProtect@ToolBlock@@YAXP6IXXZH@Z" );
	mFunctions.Insert( "?ResetCount@PacketEditor@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?SetNotifyDetectionScore@Debugger@ToolBlock@@YAXI@Z" );
	mFunctions.Insert( "?TriggerNotification@Debugger@ToolBlock@@YAXXZ" );
	mFunctions.Insert( "?getScore@CCheck@Debugger@ToolBlock@@UAEIXZ" );
	mFunctions.Insert( "?isDebuggerDetected@CCheck@Debugger@ToolBlock@@UAE_NXZ" );
	mFunctions.Insert( "?setScore@CCheck@Debugger@ToolBlock@@UAEXI@Z" );

	FILE* fh = fopen( "D:\\Apps\\Ragray\\harmony\\younas.pkg", "rb" );

	fseek( fh, 0, SEEK_END );
	unsigned int fsize = ftell(fh);
	fseek( fh, 0, SEEK_SET );

	unsigned int offset = 0;
	unsigned int dunno = 0;
	unsigned int iid = 4;

	char outpath[1024];

	while( !feof(fh) ) {
		fread( &offset, 4, 1, fh );
		fread( &dunno, 4, 1, fh );

		unsigned int oxor = calc_xor( iid );
		unsigned int oreal = oxor ^ offset;

		unsigned int ofull = dunno ^ 0x1B642C18;
		unsigned int ofreal = ofull;

		char* dedata = new char[ ofull ];
		char* data = new char[ oreal - 4 ];
		fread( data, 1, oreal - 4, fh );

		int retval = uncompress( (Bytef*)dedata, (uLongf*)&ofreal, (const Bytef*)data, oreal - 4 );			
#if 0
		sprintf( outpath, "D:\\Apps\\Ragray\\harmony\\export\\younas.%02d.bin", iid );
		FILE* fh2 = fopen( outpath, "wb" );
		fwrite( dedata, 1, ofreal, fh2 );
		fclose( fh2 );
#endif
		{//if( iid == 27 ) {
			unsigned int DllId = *(unsigned int*)(dedata+0x118);
			unsigned int FirstSectionOff = *(unsigned int*)(dedata+0x11c);
			unsigned int FullVirtSize = *(unsigned int*)(dedata+0x120);
			unsigned int SectionCount = *(unsigned int*)(dedata+0x124);
			unsigned int NumRelocations = *(unsigned int*)(dedata+0x128);
			unsigned int NumImports = *(unsigned int*)(dedata+0x12c);
			unsigned int NumExports = *(unsigned int*)(dedata+0x130);
			unsigned int NumExtraModules = *(unsigned int*)(dedata+0x134);
			unsigned int EntryPoint = *(unsigned int*)(dedata+0x138);

			unsigned int* ExtraModules = (unsigned int*)(dedata+0x13c);
			unsigned int* Relocations = 0;
			unsigned int* Imports = 0;
			unsigned int* Exports = 0;

			char* XRelocations = *(char**)&ExtraModules;
			for( int i = 0; i < NumExtraModules; i++ ) {
				XRelocations += 4;
				AddModule( XRelocations );
				XRelocations += strlen(XRelocations) + 1;
			}
			Relocations = (unsigned int*)XRelocations;
			Imports = Relocations + NumRelocations;
			Exports = Imports + NumImports * 2;

			char* odata = new char[ FullVirtSize ];
			memset( odata, 0, FullVirtSize );

			printf( "[%02d] VSIZE:%08x RSIZE:%08x CSIZE:%08x | %08x => %s\n", iid, FullVirtSize, ofreal, oreal - 4, DllId, mModules.Get(DllId).c_str() );			

			typedef std::unordered_map< unsigned int, unsigned int > ImpFuncList;
			typedef std::pair< unsigned int, unsigned int > ImpFunc;
			typedef std::unordered_map< unsigned int, ImpFuncList > ImpModuleList;
			typedef std::pair< unsigned int, ImpFuncList > ImpModule;

			typedef std::unordered_map< unsigned int, unsigned int > ExpFuncList;
			typedef std::pair< unsigned int, unsigned int > ExpFunc;

			ImpModuleList rImports;
			ExpFuncList rExports;

			unsigned int* IPtr = Imports;
			unsigned int CurrentModule;
			unsigned int FuncId = 0;
			for( int i = 0; i < NumImports; i++ ) {
				unsigned int FirstVal = *IPtr++;
				unsigned int SecondVal = *IPtr++;

				if( SecondVal == 0 ) {
					// Module
					rImports.insert( ImpModule( FirstVal, ImpFuncList() ) );
					CurrentModule = FirstVal;
				} else {
					// Import
					rImports[ CurrentModule ].insert( ImpFunc( FirstVal, SecondVal ) );
				
					FuncId++;
					if( FuncId >= NumImports )
						break;
				}
			}

			unsigned int* EPtr = Exports;
			for( int i = 0; i < NumExports; i++ ) {
				unsigned int FuncIid = *EPtr++;
				unsigned int FuncAddr = *EPtr++;

				rExports.insert( ExpFunc( FuncIid, FuncAddr ) );
			}

			printf( "  ::IMPORTS\n" );
			for( ImpModuleList::iterator i = rImports.begin( ); i != rImports.end( ); i++ )
			{
				printf( "    Module %08x => %s\n", (*i).first, mModules.Get((*i).first).c_str() );
				for( ImpFuncList::iterator j = (*i).second.begin( ); j != (*i).second.end( ); j++ )
				{
					printf( "      %08x = %08x => %s \n", (*j).second, (*j).first, mFunctions.Get((*j).first).c_str() );
				}
			}

			printf( "  ::EXPORTS\n" );
			for( ExpFuncList::iterator i = rExports.begin( ); i != rExports.end( ); i++ )
			{
				printf( "    %08x = %08x => %s\n", (*i).second, (*i).first, mFunctions.Get((*i).first).c_str() );
			}

			printf( "  ::FUNCTIONS\n" );
			printf( "    %08x - EntryPoint\n", EntryPoint );

			printf( "  ::SECTIONS\n" );
			char* ThisSection = dedata + FirstSectionOff;
			for( int i = 0; i < (int)SectionCount; i++ ) {
				unsigned char SectionFlags = *(unsigned char*)(ThisSection+0);
				int SectionVirtAddr = *(unsigned int*)(ThisSection+1);
				int SectionVirtSize = *(unsigned int*)(ThisSection+5);
				int SectionSize = *(unsigned int*)(ThisSection+9);
				int SectionAddr = (ThisSection+13)-dedata;

				memcpy( odata + SectionVirtAddr, ThisSection+13, SectionSize );

				printf( "    [%d] FLAGS:%02x VADR:%08x VLEN:%08x RADR:%08x RLEN:%08x\n", i, SectionFlags, SectionVirtAddr, SectionVirtSize, SectionAddr, SectionSize );

				ThisSection += 13 + SectionSize;
			}

#if 0
	sprintf( outpath, "D:\\Apps\\Ragray\\harmony\\export\\younas.%02d.mem", iid );
	FILE* fh3 = fopen( outpath, "wb" );
	fwrite( odata, 1, FullVirtSize, fh3 );
	fclose( fh3 );
#endif
			delete[] odata;
		}

		if( ofreal != ofull ) {
			printf( "ERROR!!!   HARMONY GAVE US SHITTY VALES!!!\n" );
		}

		delete[] data;
		delete[] dedata;
		
		iid++;
		
		if( ftell(fh) == fsize ) break;
	}

	fclose( fh );

	if( argc <= 1 ) {
		system( "PAUSE" );
	}
	return 0;
}