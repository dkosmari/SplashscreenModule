#include <exception>
#include "gfx/SplashScreenDrawer.h"
#include "gfx/gfx.h"
#include <optional>
#include "utils/logger.h"
#include "utils/utils.h"
#include "version.h"

#define MODULE_VERSION      "v0.3"
#define MODULE_VERSION_FULL MODULE_VERSION SPLASHSCREEN_MODULE_VERSION_EXTRA

int32_t main(int32_t argc, char **argv) {
    initLogging();
    DEBUG_FUNCTION_LINE_INFO("Running SplashScreen Module " MODULE_VERSION_FULL "");

    using std::filesystem::path;
    path envDir;
    if (argc >= 1) {
        envDir = argv[0];
    }

    GfxInit();
    {
        std::optional<SplashScreenDrawer> splashScreenDrawer;
        for (const auto& dir : {envDir, path{"fs:/vol/external01/wiiu"}}) {
            try {
                splashScreenDrawer.emplace(dir);
                break;
            }
            catch (std::exception &e) {
                DEBUG_FUNCTION_LINE_INFO("Failed to use %s: %s", dir.c_str(), e.what());
            }
        }
        // Fallback: use built-in empty splash.
        if (!splashScreenDrawer) {
            splashScreenDrawer.emplace();
        }
        splashScreenDrawer->Draw();

    }
    GfxShutdown();

    deinitLogging();

    return 0;
}
