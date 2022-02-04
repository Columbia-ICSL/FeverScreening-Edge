// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: interfaces/flirone_image.proto

#include "interfaces/flirone_image.pb.h"
#include "interfaces/flirone_image.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>

static const char* FlirOneImageService_method_names[] = {
  "/FlirOneImageService/SubmitImage",
};

std::unique_ptr< FlirOneImageService::Stub> FlirOneImageService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< FlirOneImageService::Stub> stub(new FlirOneImageService::Stub(channel));
  return stub;
}

FlirOneImageService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_SubmitImage_(FlirOneImageService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status FlirOneImageService::Stub::SubmitImage(::grpc::ClientContext* context, const ::FlirOneImage& request, ::common::Void* response) {
  return ::grpc::internal::BlockingUnaryCall< ::FlirOneImage, ::common::Void, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SubmitImage_, context, request, response);
}

void FlirOneImageService::Stub::experimental_async::SubmitImage(::grpc::ClientContext* context, const ::FlirOneImage* request, ::common::Void* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::FlirOneImage, ::common::Void, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitImage_, context, request, response, std::move(f));
}

void FlirOneImageService::Stub::experimental_async::SubmitImage(::grpc::ClientContext* context, const ::FlirOneImage* request, ::common::Void* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SubmitImage_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::common::Void>* FlirOneImageService::Stub::PrepareAsyncSubmitImageRaw(::grpc::ClientContext* context, const ::FlirOneImage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::common::Void, ::FlirOneImage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SubmitImage_, context, request);
}

::grpc::ClientAsyncResponseReader< ::common::Void>* FlirOneImageService::Stub::AsyncSubmitImageRaw(::grpc::ClientContext* context, const ::FlirOneImage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSubmitImageRaw(context, request, cq);
  result->StartCall();
  return result;
}

FlirOneImageService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FlirOneImageService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FlirOneImageService::Service, ::FlirOneImage, ::common::Void, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](FlirOneImageService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::FlirOneImage* req,
             ::common::Void* resp) {
               return service->SubmitImage(ctx, req, resp);
             }, this)));
}

FlirOneImageService::Service::~Service() {
}

::grpc::Status FlirOneImageService::Service::SubmitImage(::grpc::ServerContext* context, const ::FlirOneImage* request, ::common::Void* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

