cmake_minimum_required(VERSION 3.30)

project(
	MeetXRe
	LANGUAGES CXX C
	VERSION 0.1
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-unknown-attributes -Wno-return-type")
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(PostgreSQL REQUIRED)

add_subdirectory(thirdParty/json)
add_subdirectory(thirdParty/tgbot)
add_subdirectory(thirdParty/pqxx)
add_subdirectory(thirdParty/spdlog)

include_directories(ArgsHandler/include)
include_directories(Definitions/include)
include_directories(include)
include_directories(MeetXLogic/include)
include_directories(MultilanguageSystem/include)
include_directories(PSQLWrapper/include)
include_directories(MeetXLogic/Runners/include)
include_directories(MeetXLogic/Actions/include)
include_directories(thirdParty/json/include)
include_directories(thirdParty/pqxx/include)
include_directories(thirdParty/spdlog/include)

file(GLOB ArgsHandler_INC "ArgsHandler/include/*.hpp")
file(GLOB ArgsHandler_SRC "ArgsHandler/src/*.cpp")

file(GLOB Actions_INC "MeetXLogic/Actions/include/*.hpp")
file(GLOB Actions_SRC "MeetXLogic/Actions/src/*.cpp")

file(GLOB Definitions_INC "Definitions/include/*.hpp")
file(GLOB Definitions_SRC "Definitions/src/*.cpp")

file(GLOB MeetX_INC "include/*.hpp")
file(GLOB MeetX_SRC "src/*.cpp")

file(GLOB MeetXLogic_INC "MeetXLogic/include/*.hpp")
file(GLOB MeetXLogic_SRC "MeetXLogic/src/*.cpp")

file(GLOB MultilanguageSystem_INC "MultilanguageSystem/include/*.hpp")
file(GLOB MultilanguageSystem_SRC "MultilanguageSystem/src/*.cpp")

file(GLOB PSQLWrapper_INC "PSQLWrapper/include/*.hpp")
file(GLOB PSQLWrapper_SRC "PSQLWrapper/src/*.cpp")

file(GLOB Runners_INC "MeetXLogic/Runners/include/*.hpp")
file(GLOB Runners_SRC "MeetXLogic/Runners/src/*.cpp")

add_executable(MeetXRe
    ${ArgsHandler_INC}
    ${ArgsHandler_SRC}

	${Actions_INC}
	${Actions_SRC}

    ${Definitions_INC}
    ${Definitions_SRC}

    ${MeetX_INC}
    ${MeetX_SRC}

    ${MeetXLogic_INC}
    ${MeetXLogic_SRC}

    ${MultilanguageSystem_INC}
    ${MultilanguageSystem_SRC}

    ${PSQLWrapper_INC}
    ${PSQLWrapper_SRC}

    ${Runners_INC}
    ${Runners_SRC}
)

target_link_libraries(MeetXRe PRIVATE spdlog::spdlog libpqxx::pqxx nlohmann_json::nlohmann_json ${PostgreSQL_LIBRARIES} TgBot ${Boost_LIBRARIES})
