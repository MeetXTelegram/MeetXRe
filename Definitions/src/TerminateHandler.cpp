#include <Definitions.hpp>

#include <poll.h>

#include <cstdint>
#include <csetjmp>
#include <iostream>

#include <spdlog/spdlog.h>

void exceptionsUtils::TerminateHandler() {
    auto logger = spdlog::get("TerminateHandler");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The “TerminalHandler” logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    auto currentException = std::current_exception();
    if (currentException) {
        try {
            std::rethrow_exception(currentException);
        } catch (std::exception& exception) {
            logger->log(spdlog::level::critical, "An exception was received, what(): {}", exception.what());
        } catch (...) {

        }
    } else {
        logger->log(spdlog::level::critical, "No exception was found(maybe std::terminate was called directly?).");
    }

    logger->log(spdlog::level::info, "What to do?\n\n1: Correct shutdown of MeetX\n2: Try to restore the state of MeetX from the buffer");
    static std::uint16_t attempts = 2;
    while (true) {
        struct pollfd fds[1];
        std::uint16_t action;
        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;
        int result = poll(fds, 1, 5000);
        if (result > 0) {
            if (fds[0].revents & POLLIN) {
                if (!std::cin >> action)
                    action = 2;
            }
        } else if (result == 0) {
            logger->log(spdlog::level::warn, "Time is up, try again({} attempts left)", attempts--);
            continue;
        }

        switch (action) {
            case 1: {
                logger->log(spdlog::level::info, "Selected action 1(shutdown)");
                spdlog::shutdown();
                std::exit(-1);
            }

            case 2: {
                logger->log(spdlog::level::info, "Selected action 2(continue)");
                if (signalsUtils::programBuf[0].__mask_was_saved)
                    siglongjmp(signalsUtils::programBuf, 1);
                else {
                    spdlog::get("TerminateHandler")->log(spdlog::level::critical, "State recovery error: it looks like programBuffer was not initialized correctly(__jmp_buf_tag::__mask_was_saved == {})", signalsUtils::programBuf[0].__mask_was_saved);
                    spdlog::shutdown();
                    std::exit(-1);
                }
            }

            default: {
                logger->log(spdlog::level::info, "Invalid action provided, please try again");
                continue;
            }
        }
    }
}
