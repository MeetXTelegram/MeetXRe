#include <Definitions.hpp>

#include <csetjmp>
#include <iostream>

void signalsUtils::SIGUSR1Handler(int signal) {
    if (signal != SIGUSR1) {
        spdlog::get("SignalHandler")->log(spdlog::level::warn, "Invalid signal provided({}) but expected {}", signal, SIGUSR1);
        return;
    }
    spdlog::get("SignalHandler")->log(spdlog::level::warn, "The operation of MeetX was interrupted by signal {}, what should i do?\n\n1 -- Normal shutdown of MeetX\n2 -- Try to continue working", signal);
    while (true) {
        int action; std::cin >> action;
        switch (action) {
            case 1: {
                spdlog::get("SignalHandler")->log(spdlog::level::info, "Selected action: 1(shutdown)");
                std::exit(signal);
            }

            case 2: {
                spdlog::get("SignalHandler")->log(spdlog::level::info, "Selected action: 2(continue)");
                if (programBuf[0].__mask_was_saved)
                    siglongjmp(programBuf, 1);
                else {
                    spdlog::get("SignalHandler")->log(spdlog::level::critical, "State recovery error: it looks like programBuffer was not initialized correctly(__jmp_buf_tag::__mask_was_saved == {})", programBuf[0].__mask_was_saved);
                    std::exit(signal);
                }
            }

            default: {
                spdlog::get("SignalHandler")->log(spdlog::level::warn, "Invalid action provided, please try again");
                continue;
            }
        }
    }
}
