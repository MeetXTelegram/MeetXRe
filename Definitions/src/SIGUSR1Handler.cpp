#include <Definitions.hpp>

#include <csetjmp>
#include <iostream>

#include <spdlog/spdlog.h>

void signalsUtils::SIGUSR1Handler(int signal) {
    auto logger = spdlog::get("SignalHandler");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"SignalHandler\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }
    if (signal != SIGUSR1) {
        logger->log(spdlog::level::warn, "Invalid signal provided({}) but expected {}", signal, SIGUSR1);
        return;
    }
    logger->log(spdlog::level::warn, "The operation of MeetX was interrupted by signal {}, what should i do?\n\n1 -- Normal shutdown of MeetX\n2 -- Try to continue working", signal);
    while (true) {
        int action; std::cin >> action;
        switch (action) {
            case 1: {
                logger->log(spdlog::level::info, "Selected action: 1(shutdown)");
                spdlog::shutdown();
                std::exit(signal);
            }

            case 2: {
                logger->log(spdlog::level::info, "Selected action: 2(continue)");
                if (programBuf[0].__mask_was_saved)
                    siglongjmp(programBuf, 1);
                else {
                    logger->log(spdlog::level::critical, "State recovery error: it looks like programBuffer was not initialized correctly(__jmp_buf_tag::__mask_was_saved == {})", programBuf[0].__mask_was_saved);
                    spdlog::shutdown();
                    std::exit(signal);
                }
            }

            default: {
                logger->log(spdlog::level::warn, "Invalid action provided, please try again");
                continue;
            }
        }
    }
}