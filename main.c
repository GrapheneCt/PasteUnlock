#include <kernel.h>
#include <libsysmodule.h>
#include <taihen.h>

static tai_hook_ref_t hook_ref;
static SceUID hook_id;

int sceSysmoduleLoadModuleInternalWithArg_patched(SceSysmoduleInternalModuleId id, SceSize args, void *argp, void *unk) {
	int res = TAI_CONTINUE(int, hook_ref, id, args, argp, unk);
	if (res >= 0 && id == 0x80000008) {
		tai_module_info_t info;
		info.size = sizeof(info);
		taiGetModuleInfo("ScePaf", &info);

		//This makes "Select"/"Select All" work properly
		taiInjectData(info.modid, 0, 0x136262, "\x05\x22\x00\xBF", 4);
		taiInjectData(info.modid, 0, 0x136252, "\x52\xF0\x0D\x00", 4);

		//This forces copypaste feature to be enabled everywhere
		taiInjectData(info.modid, 0, 0x131262, "\x50\xF0\x01\x00", 4);
		taiInjectData(info.modid, 0, 0x13508C, "\x50\xF0\x04\x00", 4);

		taiHookRelease(hook_id, hook_ref);
	}
	return res;
}

int module_start(SceSize args, const void * argp)
{
	hook_id = taiHookFunctionImport(&hook_ref, TAI_MAIN_MODULE, 0x03FCF19D, 0xC3C26339, sceSysmoduleLoadModuleInternalWithArg_patched);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, const void * argp)
{
	return SCE_KERNEL_STOP_SUCCESS;
}