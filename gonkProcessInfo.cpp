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
#include <binder/IProcessInfoService.h>

namespace android {

class BnProcessInfoService : public BnInterface<IProcessInfoService> {
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0) = 0;
};

class GonkProcessInfoService :
    public BinderService<GonkProcessInfoService>,
    public BnProcessInfoService {
public:
    GonkProcessInfoService();
    virtual ~GonkProcessInfoService();
    static const char *getServiceName() { return "processinfo"; }

    virtual status_t getProcessStatesFromPids( size_t length,
                                        /*in*/ int32_t* pids,
                                       /*out*/ int32_t* states) override;

    virtual status_t getProcessStatesAndOomScoresFromPids( size_t length,
                                                    /*in*/ int32_t* pids,
                                                   /*out*/ int32_t* states,
                                                   /*out*/ int32_t* scores) override;

    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                uint32_t flags) override;
};

GonkProcessInfoService::GonkProcessInfoService()
  : BnProcessInfoService() {
}

GonkProcessInfoService::~GonkProcessInfoService() {
}

status_t
GonkProcessInfoService::getProcessStatesFromPids( size_t length,
                                           /*in*/ int32_t* pids,
                                          /*out*/ int32_t* states) {
    return getProcessStatesAndOomScoresFromPids(length, pids, states, nullptr);
}

#define PROCESS_STATE_UNKNOWN -1;
#define PROCESS_STATE_TOP_SLEEPING 13;
#define PROCESS_STATE_NONEXISTENT 19;
status_t
GonkProcessInfoService::getProcessStatesAndOomScoresFromPids( size_t length,
                                                       /*in*/ int32_t* pids,
                                                      /*out*/ int32_t* states,
                                                      /*out*/ int32_t* scores) {
    // sanity check for parameters
    if (length <= 0 || !pids || !states) {
        return -ENOSYS;
    }

    for (int i = 0; i < length; i++) {
        FILE *fp;
        char cmd[80], result[80];
        { /* handle process states */
            sprintf(cmd, "cat proc/%d/stat", pids[i]);
            /* Open the command for reading. */
            fp = popen(cmd, "r");
            if (fp == NULL) {
                return -ENOSYS;
            }

            /* Read the output a argument each time - output it. */
            fscanf(fp, "%s", result); /* process id, skip */
            fscanf(fp, "%s", result); /* process name, skip */
            while(!strchr(result, ')')) { /* keep finding ')' for the end of name */
                fscanf(fp, "%s", result); /* process name, skip */
            }
            fscanf(fp, "%s", result); /* process state */
            states[i] = PROCESS_STATE_NONEXISTENT;
            if (result[0] != '\0') {
                switch(result[0]) {
                    case 'S':
                        states[i] = PROCESS_STATE_TOP_SLEEPING;
                        break;
                    default:
                        states[i] = PROCESS_STATE_UNKNOWN;
                }
            }
            pclose(fp);
        }

        if (scores) { /* handle process oom_score */
            sprintf(cmd, "cat proc/%d/oom_adj", pids[i]);
            /* Open the command for reading. */
            fp = popen(cmd, "r");
            if (fp == NULL) {
                return -ENOSYS;
            }

            /* Read the output a argument each time - output it. */
            fscanf(fp, "%s", result); /* process oom_score */
            if (result[0] != '\0') {
                scores[i] = atoi(result);
            }
            pclose(fp);
        }
    }

    return NO_ERROR;
}

status_t
GonkProcessInfoService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                   uint32_t flags) {
    switch (code) {
        case GET_PROCESS_STATES_FROM_PIDS: {
            CHECK_INTERFACE(IProcessInfoService, data, reply);
            int32_t length = data.readInt32();
            if (length <= 0) {
                return NO_ERROR;
            }
            int32_t pids[length];
            status_t result = data.read((void*)pids, length * sizeof(int32_t));
            if (result != NO_ERROR) {
                return result;
            }
            int32_t statesLength = data.readInt32();
            if (length != statesLength) {
                return -ENOSYS;
            }
            int32_t states[length];
            result = getProcessStatesFromPids(length, pids, states);
            if (result != NO_ERROR) {
                return result;
            }
            reply->writeNoException();
            reply->writeInt32Array(statesLength, states);
            reply->writeInt32(result);
            return NO_ERROR;
            break;
        }
        case GET_PROCESS_STATES_AND_OOM_SCORES_FROM_PIDS: {
            CHECK_INTERFACE(IProcessInfoService, data, reply);
            int32_t length = data.readInt32();
            if (length <= 0) {
                return NO_ERROR;
            }
            int32_t pids[length];
            status_t result = data.read((void*)pids, length * sizeof(int32_t));
            if (result != NO_ERROR) {
                return result;
            }
            int32_t statesLength = data.readInt32();
            int32_t scoresLength = data.readInt32();
            if (length != statesLength || statesLength != scoresLength) {
                return -ENOSYS;
            }
            int32_t states[length], scores[length];
            result = getProcessStatesAndOomScoresFromPids(length, pids, states, scores);
            if (result != NO_ERROR) {
                return result;
            }
            reply->writeNoException();
            reply->writeInt32Array(statesLength, states);
            reply->writeInt32Array(scoresLength, scores);
            reply->writeInt32(result);
            return NO_ERROR;
            break;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}
}; // namespace android

using namespace android;

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    GonkProcessInfoService::publishAndJoinThreadPool(true);
    return 0;
}
