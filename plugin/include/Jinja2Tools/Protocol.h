#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>
namespace jinja2tools { inline constexpr int ProtocolVersion=1; enum class Operation{Ping,Validate,Format,Analyze,Shutdown}; struct DocumentPayload{std::string name,text;}; struct Diagnostic{std::string severity,code,message,source;std::optional<int> line,column,endLine,endColumn;std::optional<std::string> positionAccuracy,exceptionType;}; struct HelperRequest{int protocolVersion{ProtocolVersion};std::string requestId;Operation operation;DocumentPayload document;}; struct HelperResponse{int protocolVersion{};std::string requestId;bool success{};std::vector<Diagnostic> diagnostics;nlohmann::json result;}; void to_json(nlohmann::json&,const DocumentPayload&);void to_json(nlohmann::json&,const HelperRequest&);void from_json(const nlohmann::json&,Diagnostic&);void from_json(const nlohmann::json&,HelperResponse&);std::string operationToString(Operation);std::string createRequestId(); }
