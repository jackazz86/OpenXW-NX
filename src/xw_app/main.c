#include "xw_app/application.h"

#include "aeron/main.h"

#include <stdio.h>

#ifdef __SWITCH__
#include <SDL3/SDL_log.h>
#include <sys/stat.h>
static void SwitchLogOutput(void* user, int category, SDL_LogPriority priority, const char* message) {
	(void)user;
	(void)category;
	FILE* f = fopen("/switch/openxw/log.txt", "a");
	if (f) {
		fprintf(f, "[%d] %s\n", (int)priority, message);
		fclose(f);
	}
}
#endif

int main(int argc, char* argv[]) {
	XwLaunchOptions options;
	int valid = XwLaunchOptions_Parse(argc, argv, &options);
#ifdef __SWITCH__
	FILE* truncate_log = fopen("/switch/openxw/log.txt", "w");
	if (truncate_log)
		fclose(truncate_log);
	mkdir("/switch/openxw/user", 0777);
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
	SDL_SetLogOutputFunction(SwitchLogOutput, NULL);
	SDL_Log("OpenXW main: valid=%d", valid);
	if (valid && !options.xw98_data)
		options.xw98_data = "/switch/openxw/game";
	{
		struct stat nx_st;
		if (valid && !options.xw94_data && stat("/switch/openxw/game94", &nx_st) == 0)
			options.xw94_data = "/switch/openxw/game94";
		if (valid && !options.xw93_data && stat("/switch/openxw/game93", &nx_st) == 0)
			options.xw93_data = "/switch/openxw/game93";
	}
#endif
	if (!valid || options.show_help) {
		fprintf(valid ? stdout : stderr,
				"OpenXW\n"
				"Usage: OpenXW [options]\n"
				"  --xw93-data <path>          Extracted X-Wing 1993 B-Wing installation folder\n"
				"  --xw94-data <path>          X-Wing Collector's CD-ROM (1994) folder or image\n"
				"  --xw98-data <path>          X-Wing 1998 installation folder\n"
				"  --setup                     Choose and save your game folders\n"
				"  --skip-intro                Skip the startup sequence\n"
				"  --save-config               Save the supplied game folders\n"
				"  --reset-config              Explicitly replace config.yaml with defaults\n"
				"  --check-installation       Validate/setup from CLI and exit without a window\n"
				"  --resource-root <directory> Override packaged resources\n"
				"  --help                      Show this help\n");
		return valid ? 0 : 2;
	}
#ifdef __SWITCH__
	int rc = XwApplication_Run(&options);
	SDL_Log("OpenXW exit code %d", rc);
	return rc;
#else
	return XwApplication_Run(&options);
#endif
}
