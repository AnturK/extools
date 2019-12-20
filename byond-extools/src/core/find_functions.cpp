#include "find_functions.h"

#ifdef _WIN32
#define BYONDCORE "byondcore.dll"
#else
#define BYONDCORE "libbyond"
#endif

#define FIND_OR_DIE(name, sig) name = (name##Ptr)Pocket::Sigscan::FindPattern(BYONDCORE, sig); if(!name) { Core::Alert("Failed to locate " #name); return false; }
#ifdef _WIN32
#define IMPORT_OR_DIE(name, sig) name = (name##Ptr)GetProcAddress(LoadLibraryA(BYONDCORE), sig); if(!name) { Core::Alert("Failed to locate " #name " via " #sig); return false; }
#else
#define IMPORT_OR_DIE(name, sig) name = (name##Ptr)dlsym(dlopen(BYONDCORE, 0), sig); if(!name) { Core::Alert("Failed to locate " #name " via " #sig); return false; }
#endif

bool Core::verify_compat()
{
	IMPORT_OR_DIE(GetByondVersion, "?GetByondVersion@ByondLib@@QAEJXZ");
	IMPORT_OR_DIE(GetByondBuild, "?GetByondBuild@ByondLib@@QAEJXZ");
	ByondVersion = GetByondVersion();
	ByondBuild = GetByondBuild();
	if (ByondVersion != COMPATIBLE_MAJOR || ByondBuild < COMPATIBLE_MINOR)
	{
		Core::Alert("Incompatible BYOND version, major must = " + std::to_string(COMPATIBLE_MAJOR) + " and minor must >= " + std::to_string(COMPATIBLE_MINOR) + "(Current: "+std::to_string(ByondVersion)+"."+std::to_string(ByondBuild)+")");
		return false;
	}
	return true;
}

bool Core::find_functions()
{	
#ifdef _WIN32
	FIND_OR_DIE(Suspend, "55 8B EC 53 56 57 8B 7D 08 57 E8 ?? ?? ?? ?? 8B 1F 8B F0 8A 4F 63 83 C4 04 8B 56 18 88 4A 63 8B 4B 20 89 4E 20 8B 43 24 89 46 24 8B 45 0C C6 47 63 00 C7 43 ?? ?? ?? ?? ?? C7 43 ?? ?? ?? ?? ?? 8B 4E 18 89 41 04 F6 43 04 10");
	FIND_OR_DIE(StartTiming, "55 8B EC 56 8B 75 ?? 56 80 4E ?? ??");
	FIND_OR_DIE(SetVariable, "55 8B EC 8B 4D 08 0F B6 C1 48 57 8B 7D 10 83 F8 53 0F ?? ?? ?? ?? ?? 0F B6 80 ?? ?? ?? ?? FF 24 85 ?? ?? ?? ?? FF 75 18 FF 75 14 57 FF 75 0C E8 ?? ?? ?? ?? 83 C4 10 5F 5D C3");
	FIND_OR_DIE(GetProcArrayEntry, "55 8B EC 8B 45 08 3B 05 ?? ?? ?? ?? 72 04 33 C0 5D C3 8D 0C C0 A1 ?? ?? ?? ?? 8D 04 88 5D C3");
	FIND_OR_DIE(GetStringTableEntry, "55 8B EC 8B 4D 08 3B 0D ?? ?? ?? ?? 73 10 A1");
	FIND_OR_DIE(GetProfileInfo, "55 8B EC A1 ?? ?? ?? ?? 56 8B 75 08 3B F0 73 30 8B ?? ?? ?? ?? ?? 85 C9 75 36 8D 04 80 C1 E0 03 50 FF 15 ?? ?? ?? ?? 83 C4 04 A3 ?? ?? ?? ?? 85 C0 75 12 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 33 C0 5E 5D C3 E8 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 8D 04 B6 8D 04 C1 5E 5D C3");
	FIND_OR_DIE(CallGlobalProc, "55 8B EC 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 ?? 8B 55 ?? 8B 45 ??");
	FIND_OR_DIE(CrashProc, "55 8B EC 6A ?? 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 ?? ?? ?? ?? A1 ?? ?? ?? ?? A8 ?? 75 ?? 83 C8 ?? A3 ?? ?? ?? ?? B9 ?? ?? ?? ?? C7 45 ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 ?? C7 45 ?? ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 45 ??")
	FIND_OR_DIE(GetVariable, "55 8B EC 8B 4D ?? 0F B6 C1 48 83 F8 ?? 0F 87 ?? ?? ?? ?? 0F B6 80 ?? ?? ?? ?? FF 24 85 ?? ?? ?? ?? FF 75 ?? FF 75 ?? E8 ?? ?? ?? ??");
	FIND_OR_DIE(ProcCleanup, "55 8B EC 53 8B ?? ?? ?? ?? ?? 56 8B 75 08 57 8B 7E 44 85 FF 74 2A 8B 47 1C 89 46 44 FF 77 08 FF 37 E8 ?? ?? ?? ?? 8D 47 10 50 E8 ?? ?? ?? ?? FF 37 FF D3 57 FF D3 8B 7E 44 83 C4 14 85 FF 75 D6 FF 76 50 FF 76 48 E8");
	FIND_OR_DIE(CreateContext, "55 8B EC 83 EC 64 56 57 8B 7D 08 8B 4F 18 85 C9 0F 84 ?? ?? ?? ?? A1 ?? ?? ?? ?? 89 41 04 8B 4F 18 89 ?? ?? ?? ?? ?? 80 79 69 00");
	FIND_OR_DIE(GetTypeById, "55 8B EC 8B 45 08 3B 05 ?? ?? ?? ?? 72 04 33 C0 5D C3 6B C0 64 03 05 ?? ?? ?? ?? 5D C3");
	FIND_OR_DIE(MobTableIndexToGlobalTableIndex, "55 8B EC 8B 45 08 3B 05 ?? ?? ?? ?? 72 04 33 C0 5D C3 C1 E0 04 03 05 ?? ?? ?? ?? 5D C3");
	FIND_OR_DIE(Runtime, "55 8B EC 6A FF 68 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 50 83 EC 40 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 ?? ?? ?? ?? ?? ?? 89 65 F0 A1");
	FIND_OR_DIE(GetTurf, "55 8B EC 8B 45 08 0F BF ?? ?? ?? ?? ?? 53 56 0F B7 F0 57 3B F2 7D 3A 8B 45 0C 0F BF ?? ?? ?? ?? ?? 0F B7 D8 3B DF 7D 29");
	FIND_OR_DIE(AppendToContainer, "55 8B EC 8B 4D 08 0F B6 C1 48 56 83 F8 53 0F 87 ?? ?? ?? ?? 0F B6 ?? ?? ?? ?? ?? FF 24 ?? ?? ?? ?? ?? FF 75 0C E8 ?? ?? ?? ?? 8B F0 83 C4 04 85 F6 0F 84 ?? ?? ?? ?? 8B 46 0C 40 50 56 E8 ?? ?? ?? ?? 8B 56 0C 83 C4 08 85 D2");
	FIND_OR_DIE(GetAssocElement, "55 8B EC 51 8B 4D 08 C6 45 FF 00 80 F9 05 76 11 80 F9 21 74 10 80 F9 0D 74 0B 80 F9 0E 75 65 EB 04 84 C9 74 5F 6A 00 8D 45 FF 50 FF 75 0C 51 6A 00 6A 7B");
	FIND_OR_DIE(GetListPointerById, "55 8B EC FF 75 08 E8 ?? ?? ?? ?? 83 C4 04 85 C0 75 13 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 5D E9 ?? ?? ?? ?? 5D C3");
	FIND_OR_DIE(SetAssocElement, "55 8B EC 83 EC 14 8B 4D 08 C6 45 FF 00 80 F9 05 76 15 80 F9 21 74 14 80 F9 0D 74 0F 80 F9 0E 0F 85 ?? ?? ?? ?? EB 04 84 C9 74 7A 6A 00 8D 45 FF 50 FF 75 0C 51 6A 00 6A 7C");
	FIND_OR_DIE(CreateList, "55 8B EC 8B ?? ?? ?? ?? ?? 56 85 C9 74 1B A1 ?? ?? ?? ?? 49 89 ?? ?? ?? ?? ?? 8B 34 88 81 FE ?? ?? ?? ?? 0F 85 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 8B F1 81 F9 ?? ?? ?? ?? 75 1B 51 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C B8 ?? ?? ?? ?? 5E 5D C3");
	FIND_OR_DIE(Length, "55 8B EC 8B 4D 08 83 EC 18 0F B6 C1 48 53 56 57 83 F8 53 0F 87 ?? ?? ?? ?? 0F B6 ?? ?? ?? ?? ?? FF 24 ?? ?? ?? ?? ?? FF 75 0C");
	FIND_OR_DIE(IsInContainer, "55 8B EC 8B 45 10 83 EC 38 3C 0F 75 4D FF 75 14 E8 ?? ?? ?? ?? 83 C4 04 85 C0 74 3B 83 78 04 00 74 35 6A 0F FF 75 14 FF 75 10 E8");
	FIND_OR_DIE(ToString, "55 8B EC 6A FF 68 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 50 83 EC 10 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 ?? ?? ?? ?? ?? ?? 8B 5D 08 0F B6 C3 48 83 F8 53");
	switch(ByondVersion) {
		case 512:
			FIND_OR_DIE(GetStringTableIndex, "55 8B EC 8B 45 ?? 83 EC ?? 53 8B 1D ?? ?? ?? ??");
			break;
		case 513:
			FIND_OR_DIE(GetStringTableIndexUTF8, "55 8B EC 8B 45 ?? 83 EC ?? 53 56 8B 35 ?? ?? ?? ??");
			break;
		default: break;
	}
	current_execution_context_ptr = *(ExecutionContext * **)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? 8D ?? ?? ?? ?? ?? 83 C4 08 89 48 28 8D ?? ?? ?? ?? ?? 89 48 2C 83 3D ?? ?? ?? ?? ?? 74 25 8B 00 FF 30 E8 ?? ?? ?? ?? 83 C4 04 FF 30 E8 ?? ?? ?? ?? 83 C4 04 FF 30 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 66 ?? ?? ?? ?? ?? ?? A1 ?? ?? ?? ?? 75 28 A8 02 75 24 E8 ?? ?? ?? ?? 85 C0 75 09 50", 1);
	proc_setup_table = **(ProcSetupEntry * ***)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? FF 34 B8 FF D6 47 83 C4 04 3B ?? ?? ?? ?? ?? 72 EA FF 35 ?? ?? ?? ?? FF D6 33 FF 83 C4 04 39 ?? ?? ?? ?? ?? 76 1E", 1);
	some_flags_including_profile = *(unsigned int**)Pocket::Sigscan::FindPattern(BYONDCORE, "F7 05 ?? ?? ?? ?? ?? ?? ?? ?? 74 34 8B 01 FF 30 E8 ?? ?? ?? ?? 83 C4 04 8B D8 E8 ?? ?? ?? ?? 8B F0 8B FA E8 ?? ?? ?? ?? 85 DB 74 02 FF 03 8B ?? ?? ?? ?? ?? 89 71 70 89 79 74 89 41 78 89 51 7C 83 3D ?? ?? ?? ?? ?? 5B 74 37", 2);
	parent_context_ptr_hack = *(ExecutionContext***)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? 89 46 04 8B 45 E0 89 77 18 89 3E 89 ?? ?? ?? ?? ?? FF 70 18 E8 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 83 C4 10 8B 40 04 89 41 10 C7 41 ?? ?? ?? ?? ?? F6 47 04 08 8B 77 2C 89 75 F8 0F 84 ?? ?? ?? ?? 8B 06", 1);
#else
	FIND_OR_DIE(CrashProc, 	"55 89 E5 53 83 EC ?? 80 3D ?? ?? ?? ?? ?? 75 ?? C7 04 24 ?? ?? ?? ?? E8 ?? ?? ?? ?? 85 C0 75 ?? C7 04 24 ?? ?? ?? ?? 8D 5D ?? E8 ?? ?? ?? ?? 8B 45 ?? 89 5C 24 ?? C7 04 24 ?? ?? ?? ?? 89 44 24 ?? E8 ?? ?? ?? ?? C7 04 24 ?? ?? ?? ??");
	FIND_OR_DIE(Suspend, "55 89 E5 57 31 FF 56 89 C6 53 83 EC ?? 89 F0"); //regparm3
	FIND_OR_DIE(StartTiming, "55 89 E5 83 EC ?? 85 C0 89 5D ?? 89 C3 89 75 ?? 89 7D ?? 74 ?? 8B 50 ??"); //regparm3
	FIND_OR_DIE(GetVariable, "55 89 E5 81 EC ?? ?? ?? ?? 8B 55 ?? 89 5D ?? 8B 5D ?? 89 75 ?? 8B 75 ??");
	FIND_OR_DIE(GetProcArrayEntry, "55 31 C0 89 E5 8B 55 ?? 39 15 ?? ?? ?? ?? 76 ?? 8D 04 D2");
	FIND_OR_DIE(GetStringTableEntry, "55 89 E5 83 EC ?? 8B 45 ?? 39 05 ?? ?? ?? ??");
	FIND_OR_DIE(GetProfileInfo, "55 31 C0 89 E5 53 83 EC ?? 8B 5D ?? 8B 15 ?? ?? ?? ??");
	FIND_OR_DIE(CallGlobalProc, "55 89 E5 81 EC ?? ?? ?? ?? 89 5D ?? 89 C3 0F B6 45");
	FIND_OR_DIE(ProcCleanup, "55 89 E5 56 89 C6 53 83 EC ?? 8B 58 ??");
	FIND_OR_DIE(CreateContext, "55 89 E5 57 56 89 C6 53 89 D3 83 EC ?? 80 3D ?? ?? ?? ?? ??");
#error GetTypeById missing
#error MobTableIndexToGlobalTableIndex missing
	switch(ByondVersion) {
		case 512:
			FIND_OR_DIE(SetVariable, "55 89 E5 81 EC ?? ?? ?? ?? 89 75 ?? 8B 55 ?? 8B 75 ??");
			FIND_OR_DIE(GetStringTableIndex, "55 89 E5 57 56 53 89 D3 83 EC ?? 85 C0"); // regparm3
			break;
		case 513:
			FIND_OR_DIE(SetVariable, "55 89 E5 81 EC ?? ?? ?? ?? 8B 55 ?? 8B 45 ??");
			FIND_OR_DIE(GetStringTableIndexUTF8, "55 89 E5 57 56 89 CE 53 89 D3 83 EC ?? 8B 55 ?? 85 C0") // regparm3
			break;
		default: break;
	}
	current_execution_context_ptr = *(ExecutionContext * **)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? 8D 7D ?? 89 78 ??", 1);
	proc_setup_table = **(ProcSetupEntry * ***)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? 8B 04 98 85 C0 74 ?? 89 04 24 E8 ?? ?? ?? ?? 8B 15 ?? ?? ?? ??", 1);
	some_flags_including_profile = *(unsigned int**)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? A8 ?? 0F 85 ?? ?? ?? ??", 2);
	parent_context_ptr_hack = *(ExecutionContext***)Pocket::Sigscan::FindPattern(BYONDCORE, "A1 ?? ?? ?? ?? 8B 4D ?? 0F B7 50 ", 1);
#endif
	return true;
}