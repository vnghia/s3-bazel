#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/transfer/TransferManager.h>

static const char *ALLOC_TAG = "main";

int main() {
  Aws::SDKOptions options;
  options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
  Aws::InitAPI(options);

  Aws::Client::ClientConfiguration clientConfiguration;

  using namespace Aws::Transfer;
  using namespace Aws::Utils;

  auto s3Client =
      Aws::MakeShared<Aws::S3::S3Client>(ALLOC_TAG, clientConfiguration);
  auto thread_executor =
      Aws::MakeShared<Threading::PooledThreadExecutor>(ALLOC_TAG, 4);
  Aws::Transfer::TransferManagerConfiguration transferConfig(
      thread_executor.get());
  transferConfig.s3Client = s3Client;

  auto transferManager = TransferManager::Create(transferConfig);

  auto transferHandle = transferManager->DownloadFile("vnvo", "5kb", "5kb");
  transferHandle->WaitUntilFinished();
  /* you may need to reset the thread pool before shutdown API */
  thread_executor.reset();
  Aws::ShutdownAPI(options);
  return 0;
}
