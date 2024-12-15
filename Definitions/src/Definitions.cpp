#include <Definitions.hpp>

#include <csetjmp>

#include <pqxx/pqxx>

std::shared_ptr<pqxx::connection> db::database = nullptr;
std::size_t db::adminModePasswordHash = 0;

sigjmp_buf signalsUtils::programBuf;
