#include <pl/memory/Hook.h>
#include "ToastAPI.hpp"

struct StartMenuScreenController {};

LL_TYPED_HOOK(
    StartMenuScreenController_open,
    memory::HookPriority::Normal,
    StartMenuScreenController,
    pl::memory::resolveVtableFunction("25StartMenuScreenController", 7, "libminecraftpe.so"),
    "libminecraftpe.so",
    void
) {
    origin();
    nexcaise::toastapi::sendToastMessage("ToastAPI !!!!!!");
};

__attribute__((constructor))
void onLoad() { StartMenuScreenController_open::hook(); }

__attribute__((destructor))
void onUnload() { StartMenuScreenController_open::unhook(); }
