/* (c) 2019 KAI OS TECHNOLOGIES (HONG KONG) LIMITED All rights reserved. This
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
#include <android/hardware/ISensorPrivacyManager.h>
#include <android/hardware/ISensorPrivacyListener.h>

namespace android {

class BnSensorPrivacyService : public BnInterface<hardware::ISensorPrivacyManager> {
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0) = 0;
};

typedef binder::Status Status;

class FakeSensorPrivacyService :
    public BinderService<FakeSensorPrivacyService>,
    public BnSensorPrivacyService {
public:
    FakeSensorPrivacyService();
    virtual ~FakeSensorPrivacyService();
    static const char *getServiceName() { return "sensor_privacy"; }

    virtual Status addSensorPrivacyListener(const sp<hardware::ISensorPrivacyListener>& listener);
    virtual Status removeSensorPrivacyListener(const sp<hardware::ISensorPrivacyListener>& listener);
    virtual Status isSensorPrivacyEnabled(bool* _aidl_return);
    virtual Status setSensorPrivacy(bool enable);

    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                uint32_t flags) override;

};

FakeSensorPrivacyService::FakeSensorPrivacyService()
 : BnSensorPrivacyService() {
}

FakeSensorPrivacyService::~FakeSensorPrivacyService() {
}

Status
FakeSensorPrivacyService::addSensorPrivacyListener(const sp<hardware::ISensorPrivacyListener>& listener) {
    (void)listener;

    return Status::ok();
}

Status
FakeSensorPrivacyService::removeSensorPrivacyListener(const sp<hardware::ISensorPrivacyListener>& listener) {
    (void)listener;

    return Status::ok();
}

Status
FakeSensorPrivacyService::isSensorPrivacyEnabled(bool* _aidl_return) {
    /* Do not enable sensor privacy for b2g*/
    *_aidl_return = false;

    return Status::ok();
}

Status
FakeSensorPrivacyService::setSensorPrivacy(bool enable) {
    (void)enable;

    return Status::ok();
}

status_t
FakeSensorPrivacyService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                   uint32_t flags) {
    switch (code) {
        case IBinder::FIRST_CALL_TRANSACTION + 0: /* addSensorPrivacyListener */ {
            CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
            sp<hardware::ISensorPrivacyListener> listener;
            data.readStrongBinder(&listener);
            Status status(addSensorPrivacyListener(listener));
            return status.writeToParcel(reply);
        }
        case IBinder::FIRST_CALL_TRANSACTION + 1: /* removeSensorPrivacyListener */ {
            CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
            sp<hardware::ISensorPrivacyListener> listener;
            data.readStrongBinder(&listener);
            Status status(removeSensorPrivacyListener(listener));
            return status.writeToParcel(reply);
        }
        case IBinder::FIRST_CALL_TRANSACTION + 2: /* isSensorPrivacyEnabled */ {
            CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
            bool enabled;
            Status status(isSensorPrivacyEnabled(&enabled));
            status_t result = status.writeToParcel(reply);
            reply->writeBool(enabled);
            return result;
        }
        case IBinder::FIRST_CALL_TRANSACTION + 3: /* setSensorPrivacy */ {
            CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
            bool enabled;
            data.readBool(&enabled);
            Status status(setSensorPrivacy(enabled));
            return status.writeToParcel(reply);
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace android

using namespace android;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    FakeSensorPrivacyService::publishAndJoinThreadPool();
    return 0;
}
