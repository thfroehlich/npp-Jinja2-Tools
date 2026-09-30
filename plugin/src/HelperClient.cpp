#include "Jinja2Tools/HelperClient.h"

#include <stdexcept>

namespace jinja2tools
{
    HelperClient::HelperClient(
        std::filesystem::path executable,
        std::chrono::milliseconds timeout)
        : executable_(std::move(executable))
        , timeout_(timeout)
    {
    }

    HelperResponse HelperClient::execute(
        Operation operation,
        const DocumentPayload& document) const
    {
        if (!std::filesystem::is_regular_file(executable_))
        {
            throw std::runtime_error(
                "Helper executable not found: " +
                executable_.string());
        }

        HelperRequest request{
            ProtocolVersion,
            createRequestId(),
            operation,
            document};

        nlohmann::json requestJson = request;

        std::string payload =
            requestJson.dump();

        payload.push_back('\n');

        auto processResult =
            process_.execute(
                executable_,
                payload,
                timeout_);

        if (processResult.timedOut)
        {
            throw std::runtime_error(
                "Helper timed out");
        }

        if (processResult.standardOutput.empty())
        {
            throw std::runtime_error(
                "Helper returned empty stdout. stderr: " +
                processResult.standardError);
        }

        HelperResponse response;

        try
        {
            response =
                nlohmann::json::parse(
                    processResult.standardOutput)
                    .get<HelperResponse>();
        }
        catch (const std::exception& exception)
        {
            throw std::runtime_error(
                std::string("Invalid helper JSON: ") +
                exception.what() +
                "; stderr: " +
                processResult.standardError);
        }

        if (response.protocolVersion != ProtocolVersion)
        {
            throw std::runtime_error(
                "Protocol version mismatch");
        }

        if (response.requestId != request.requestId)
        {
            throw std::runtime_error(
                "Request ID mismatch");
        }

        if (processResult.exitCode != 0)
        {
            throw std::runtime_error(
                "Helper failed with exit code " +
                std::to_string(processResult.exitCode) +
                ": " +
                processResult.standardError);
        }

        return response;
    }

    HelperResponse HelperClient::ping() const
    {
        return execute(
            Operation::Ping,
            {
                "ping.j2",
                ""
            });
    }
}