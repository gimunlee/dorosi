//
//  CCCCCoreLocation.h
//
//  Created by franck on 3/19/13.
//
//

#ifndef __CCCoreLocation__
#define __CCCoreLocation__

#include <iostream>

#include "cocos2d.h"
#include <iostream>

#include "platform/android/jni/JniHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>//"C:/Program Files/Java/jdk1.8.0_92/include/jni.h"
#include <android/log.h>
#endif

#define LOCATION_KEY "LOCATION_KEY"

struct locationStruct {
    float longi;
    float lati;
};

class CCCoreLocation : public cocos2d::CCObject

{
public:
    char _LOCATION_KEY[1000];
private:
    cocos2d::SEL_CallFuncND _pSelector;
    cocos2d::CCObject* _pTarget;
    locationStruct _loc;
public:


    CCCoreLocation();
    void getLocation(cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void getLocationDone(float longi, float lati);
    bool isLocationAvailable();
    static  CCCoreLocation * getInstance();


};


#ifdef __cplusplus
 extern "C" {
#endif

void Java_org_cocos2dx_cpp_CClocation_nativeupdatePosition(JNIEnv *env, jobject thiz, jdouble a, jdouble b);

#ifdef __cplusplus
}
#endif


#endif
