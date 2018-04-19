#include "message_handler.h"
#include "queue_manager.h"

#include "cache_manager.h"
#include "config.h"

namespace {
MethodType kMethodType = "shutdown";

struct In_Shutdown : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
};
MAKE_REFLECT_STRUCT(In_Shutdown, id);
REGISTER_IN_MESSAGE(In_Shutdown);

struct Out_Shutdown : public lsOutMessage<Out_Shutdown> {
  lsRequestId id;
  JsonNull result;
};
MAKE_REFLECT_STRUCT(Out_Shutdown, jsonrpc, id, result);

struct Handler_Shutdown : BaseMessageHandler<In_Shutdown> {
  MethodType GetMethodType() const override { return kMethodType; }
  void Run(In_Shutdown* request) override {
    g_config->cacheStore->Close();
    Out_Shutdown out;
    out.id = request->id;
    QueueManager::WriteStdout(kMethodType, out);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_Shutdown);
}  // namespace
