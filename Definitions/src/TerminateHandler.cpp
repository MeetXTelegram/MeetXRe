#include <Definitions.hpp>

#include <csetjmp>
#include <iostream>

#include <spdlog/spdlog.h>

#include <Definitions.hpp>

void exceptionsUtils::TerminateHandler() {
    if (std::uncaught_exceptions() > 0) {
        spdlog::get("TerminateHandler")->log(spdlog::level::warn, "Uncaught exceptions: {}(It looks like this handler was calle as a result of throwing an exception), what should i do?\n\n1 -- Normal shutdown of MeetX\n2 -- Try to restart the process");
    } else {
        spdlog::get("TerminateHandler")->log(spdlog::level::warn, "No uncaught exceptions were found(It looks like std::terminate was called explicitly), what should i do?\n\n1 -- Normal shutdown of MeetX\n2 -- Try to continue working");
    }

    while (true) {
        int action; std::cin >> action;

        switch (action) {
            case 1: {
                spdlog::get("TerminateHandler")->log(spdlog::level::info, "Selected action 1(shutdown)");
                std::exit(-1);
            }

            case 2: {
                spdlog::get("TerminateHandler")->log(spdlog::level::info, "Selected action 2(continue)");
                if (signalsUtils::programBuf[0].__mask_was_saved)
                    siglongjmp(signalsUtils::programBuf, 1);
                else {
                    spdlog::get("TerminateHandler")->log(spdlog::level::critical, "State recovery error: it looks like programBuffer was not initialized correctly(__jmp_buf_tag::__mask_was_saved == {})", signalsUtils::programBuf[0].__mask_was_saved);
                    std::exit(-1);
                }
            }

            default: {
                spdlog::get("TerminateHandler")->log(spdlog::level::info, "Invalid action provided, please try again");
                continue;
            }
        }
    }
}
