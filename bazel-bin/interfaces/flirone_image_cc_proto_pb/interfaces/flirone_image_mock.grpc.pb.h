// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: interfaces/flirone_image.proto

#include "interfaces/flirone_image.pb.h"
#include "interfaces/flirone_image.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/sync_stream.h>
#include <gmock/gmock.h>

class MockFlirOneImageServiceStub : public FlirOneImageService::StubInterface {
 public:
  MOCK_METHOD3(SubmitImage, ::grpc::Status(::grpc::ClientContext* context, const ::FlirOneImage& request, ::common::Void* response));
  MOCK_METHOD3(AsyncSubmitImageRaw, ::grpc::ClientAsyncResponseReaderInterface< ::common::Void>*(::grpc::ClientContext* context, const ::FlirOneImage& request, ::grpc::CompletionQueue* cq));
  MOCK_METHOD3(PrepareAsyncSubmitImageRaw, ::grpc::ClientAsyncResponseReaderInterface< ::common::Void>*(::grpc::ClientContext* context, const ::FlirOneImage& request, ::grpc::CompletionQueue* cq));
};
