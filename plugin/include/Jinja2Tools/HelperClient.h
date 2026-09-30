#pragma once
#include "HelperProcess.h"
#include "Protocol.h"
namespace jinja2tools { class HelperClient final { public: HelperClient(std::filesystem::path,std::chrono::milliseconds); HelperResponse execute(Operation,const DocumentPayload&) const; HelperResponse ping() const; private: std::filesystem::path executable_;std::chrono::milliseconds timeout_;HelperProcess process_;}; }
