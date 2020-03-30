/* (c) 2020 KAI OS TECHNOLOGIES (HONG KONG) LIMITED All rights reserved. This
 * file or any portion thereof may not be reproduced or used in any manner
 * whatsoever without the express written permission of KAI OS TECHNOLOGIES
 * (HONG KONG) LIMITED. KaiOS is the trademark of KAI OS TECHNOLOGIES (HONG
 * KONG) LIMITED or its affiliate company and may be registered in some
 * jurisdictions. All other trademarks are the property of their respective
 * owners.
 */

#include <binder/BinderService.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <android/content/pm/BnPackageManagerNative.h>

namespace android {

using binder::Status;
using content::pm::BnPackageManagerNative;

// There is a case that ~AudioTrack() may update media metrics to
// MediaAnalyticsService, and it will be blocked for 5 seconds while trying to
// get the binder of "package_native" service, which doesn't exists because
// PackageManagerService.java has been removed from KaiOS. So add
// FakePackageManagerNative to prevent this problem.
class FakePackageManagerNative :
    public BinderService<FakePackageManagerNative>,
    public BnPackageManagerNative {
public:
    FakePackageManagerNative();
    virtual ~FakePackageManagerNative();
    static const char *getServiceName() { return "package_native"; }

    virtual Status getNamesForUids(const std::vector<int32_t>& uids, std::vector<std::string>* _aidl_return);
    virtual Status getInstallerForPackage(const String16& packageName, std::string* _aidl_return);
    virtual Status getVersionCodeForPackage(const String16& packageName, int64_t* _aidl_return);
    virtual Status isAudioPlaybackCaptureAllowed(const std::vector<std::string>& packageNames, std::vector<bool>* _aidl_return);
    virtual Status getLocationFlags(const std::string& packageName, int32_t* _aidl_return);
    virtual Status getTargetSdkVersionForPackage(const String16& packageName, int32_t* _aidl_return);
    virtual Status getModuleMetadataPackageName(std::string* _aidl_return);
};

FakePackageManagerNative::FakePackageManagerNative() {
}

FakePackageManagerNative::~FakePackageManagerNative() {
}

Status FakePackageManagerNative::getNamesForUids(const std::vector<int32_t>& uids, std::vector<std::string>* _aidl_return) {
    _aidl_return->assign(uids.size(), std::string());
    return Status::ok();
}

Status FakePackageManagerNative::getInstallerForPackage(const String16& packageName, std::string* _aidl_return) {
    (void)packageName;
    _aidl_return->clear();
    return Status::ok();
}

Status FakePackageManagerNative::getVersionCodeForPackage(const String16& packageName, int64_t* _aidl_return) {
    (void)packageName;
    *_aidl_return = 0;
    return Status::ok();
}

Status FakePackageManagerNative::isAudioPlaybackCaptureAllowed(const std::vector<std::string>& packageNames, std::vector<bool>* _aidl_return) {
    _aidl_return->assign(packageNames.size(), false);
    return Status::ok();
}

Status FakePackageManagerNative::getLocationFlags(const std::string& packageName, int32_t* _aidl_return) {
    (void)packageName;
    *_aidl_return = 0;
    return Status::ok();
}

Status FakePackageManagerNative::getTargetSdkVersionForPackage(const String16& packageName, int32_t* _aidl_return) {
    (void)packageName;
    *_aidl_return = 0;
    return Status::ok();
}

Status FakePackageManagerNative::getModuleMetadataPackageName(std::string* _aidl_return) {
    _aidl_return->clear();
    return Status::ok();
}

}; // namespace android

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    android::FakePackageManagerNative::publishAndJoinThreadPool();
    return 0;
}
