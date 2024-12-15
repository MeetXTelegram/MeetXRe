#include <spdlog/spdlog.h>

#include <Args.hpp>
#include <Definitions.hpp>
#include <Init.hpp>
#include <Logic.hpp>

int main(int argc, char* argv[]) {
    init::initLoggers();
    ah::Args args(argc, argv);
    args.process(ah::Args::ArgsStyle::LIKE_CMAKE);
    init::MeetXInit(args);
    meetx::MeetXStart();
    meetx::InitRunners();
    meetx::MainLoop();
}
