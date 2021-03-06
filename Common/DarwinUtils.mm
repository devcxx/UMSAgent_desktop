/*
 *      Copyright (C) 2010-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#define BOOL XBMC_BOOL

#undef BOOL

#if defined(TARGET_DARWIN)
#if defined(TARGET_DARWIN_IOS)
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <mach/mach_host.h>
#import <sys/sysctl.h>
#else
#import "AppleDevice.h"
#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#import <IOKit/IOKitLib.h>
#import <IOKit/ps/IOPSKeys.h>
#import <IOKit/ps/IOPowerSources.h>
#import <SystemConfiguration/SystemConfiguration.h>
#endif

#import "AutoPool.h"
#import "DarwinUtils.h"

#ifndef NSAppKitVersionNumber10_5
#define NSAppKitVersionNumber10_5 949
#endif

#ifndef NSAppKitVersionNumber10_6
#define NSAppKitVersionNumber10_6 1038
#endif

#ifndef NSAppKitVersionNumber10_9
#define NSAppKitVersionNumber10_9 1265
#endif

enum iosPlatform {
    iDeviceUnknown = -1,
    iPhone2G,
    iPhone3G,
    iPhone3GS,
    iPodTouch1G,
    iPodTouch2G,
    iPodTouch3G,
    iPad,
    iPad3G,
    iPad2WIFI,
    iPad2CDMA,
    iPad2,
    iPadMini,
    iPadMiniGSMCDMA,
    iPadMiniWIFI,
    AppleTV2,
    iPhone4, //from here on list devices with retina support (e.x. mainscreen scale == 2.0)
    iPhone4CDMA,
    iPhone4S,
    iPhone5,
    iPhone5GSMCDMA,
    iPhone5CGSM,
    iPhone5CGlobal,
    iPhone5SGSM,
    iPhone5SGlobal,
    iPodTouch4G,
    iPodTouch5G,
    iPad3WIFI,
    iPad3GSMCDMA,
    iPad3,
    iPad4WIFI,
    iPad4,
    iPad4GSMCDMA,
    iPadAirWifi,
    iPadAirCellular,
    iPadMini2Wifi,
    iPadMini2Cellular,
    iPhone6,
    iPadAir2Wifi,
    iPadAir2Cellular,
    iPadMini3Wifi,
    iPadMini3Cellular,
    iPhone6Plus, //from here on list devices with retina support which have scale == 3.0
};

// platform strings are based on http://theiphonewiki.com/wiki/Models
const char* getIosPlatformString(void)
{
    static std::string iOSPlatformString;
    if (iOSPlatformString.empty()) {
#if defined(TARGET_DARWIN_IOS)
        // Gets a string with the device model
        size_t size;
        sysctlbyname("hw.machine", NULL, &size, NULL, 0);
        char* machine = new char[size];
        if (sysctlbyname("hw.machine", machine, &size, NULL, 0) == 0 && machine[0])
            iOSPlatformString.assign(machine, size - 1);
        else
#endif
            iOSPlatformString = "unknown0,0";

#if defined(TARGET_DARWIN_IOS)
        delete[] machine;
#endif
    }

    return iOSPlatformString.c_str();
}

enum iosPlatform getIosPlatform()
{
    static enum iosPlatform eDev = iDeviceUnknown;
#if defined(TARGET_DARWIN_IOS)
    if (eDev == iDeviceUnknown) {
        std::string devStr(getIosPlatformString());

        if (devStr == "iPhone1,1")
            eDev = iPhone2G;
        else if (devStr == "iPhone1,2")
            eDev = iPhone3G;
        else if (devStr == "iPhone2,1")
            eDev = iPhone3GS;
        else if (devStr == "iPhone3,1")
            eDev = iPhone4;
        else if (devStr == "iPhone3,2")
            eDev = iPhone4;
        else if (devStr == "iPhone3,3")
            eDev = iPhone4CDMA;
        else if (devStr == "iPhone4,1")
            eDev = iPhone4S;
        else if (devStr == "iPhone5,1")
            eDev = iPhone5;
        else if (devStr == "iPhone5,2")
            eDev = iPhone5GSMCDMA;
        else if (devStr == "iPhone5,3")
            eDev = iPhone5CGSM;
        else if (devStr == "iPhone5,4")
            eDev = iPhone5CGlobal;
        else if (devStr == "iPhone6,1")
            eDev = iPhone5SGSM;
        else if (devStr == "iPhone6,2")
            eDev = iPhone5SGlobal;
        else if (devStr == "iPhone7,1")
            eDev = iPhone6Plus;
        else if (devStr == "iPhone7,2")
            eDev = iPhone6;
        else if (devStr == "iPod1,1")
            eDev = iPodTouch1G;
        else if (devStr == "iPod2,1")
            eDev = iPodTouch2G;
        else if (devStr == "iPod3,1")
            eDev = iPodTouch3G;
        else if (devStr == "iPod4,1")
            eDev = iPodTouch4G;
        else if (devStr == "iPod5,1")
            eDev = iPodTouch5G;
        else if (devStr == "iPad1,1")
            eDev = iPad;
        else if (devStr == "iPad1,2")
            eDev = iPad;
        else if (devStr == "iPad2,1")
            eDev = iPad2WIFI;
        else if (devStr == "iPad2,2")
            eDev = iPad2;
        else if (devStr == "iPad2,3")
            eDev = iPad2CDMA;
        else if (devStr == "iPad2,4")
            eDev = iPad2;
        else if (devStr == "iPad2,5")
            eDev = iPadMiniWIFI;
        else if (devStr == "iPad2,6")
            eDev = iPadMini;
        else if (devStr == "iPad2,7")
            eDev = iPadMiniGSMCDMA;
        else if (devStr == "iPad3,1")
            eDev = iPad3WIFI;
        else if (devStr == "iPad3,2")
            eDev = iPad3GSMCDMA;
        else if (devStr == "iPad3,3")
            eDev = iPad3;
        else if (devStr == "iPad3,4")
            eDev = iPad4WIFI;
        else if (devStr == "iPad3,5")
            eDev = iPad4;
        else if (devStr == "iPad3,6")
            eDev = iPad4GSMCDMA;
        else if (devStr == "iPad4,1")
            eDev = iPadAirWifi;
        else if (devStr == "iPad4,2")
            eDev = iPadAirCellular;
        else if (devStr == "iPad4,4")
            eDev = iPadMini2Wifi;
        else if (devStr == "iPad4,5")
            eDev = iPadMini2Cellular;
        else if (devStr == "iPad4,7")
            eDev = iPadMini3Wifi;
        else if (devStr == "iPad4,8")
            eDev = iPadMini3Cellular;
        else if (devStr == "iPad4,9")
            eDev = iPadMini3Cellular;
        else if (devStr == "iPad5,3")
            eDev = iPadAir2Wifi;
        else if (devStr == "iPad5,4")
            eDev = iPadAir2Cellular;
        else if (devStr == "AppleTV2,1")
            eDev = AppleTV2;
    }
#endif
    return eDev;
}

bool IsMavericks(void)
{
    static int isMavericks = -1;
#if defined(TARGET_DARWIN_OSX)
    // there is no NSAppKitVersionNumber10_9 out there anywhere
    // so we detect mavericks by one of these newly added app nap
    // methods - and fix the ugly mouse rect problem which was hitting
    // us when mavericks came out
    if (isMavericks == -1) {
        //    CLog::Log(LOGDEBUG, "Detected Mavericks...");
        isMavericks = [NSProcessInfo instancesRespondToSelector:@selector(beginActivityWithOptions:reason:)] == TRUE ? 1 : 0;
    }
#endif
    return isMavericks == 1;
}

bool IsSnowLeopard(void)
{
    static int isSnowLeopard = -1;
#if defined(TARGET_DARWIN_OSX)
    if (isSnowLeopard == -1) {
        double appKitVersion = floor(NSAppKitVersionNumber);
        isSnowLeopard = (appKitVersion <= NSAppKitVersionNumber10_6 && appKitVersion > NSAppKitVersionNumber10_5) ? 1 : 0;
    }
#endif
    return isSnowLeopard == 1;
}

bool DeviceHasRetina(double& scale)
{
    static enum iosPlatform platform = iDeviceUnknown;

#if defined(TARGET_DARWIN_IOS)
    if (platform == iDeviceUnknown) {
        platform = getIosPlatform();
    }
#endif
    scale = 1.0; //no retina

    // see http://www.paintcodeapp.com/news/iphone-6-screens-demystified
    if (platform >= iPhone4 && platform < iPhone6Plus) {
        scale = 2.0; // 2x render retina
    }

    if (platform >= iPhone6Plus) {
        scale = 3.0; //3x render retina + downscale
    }

    return (platform >= iPhone4);
}

bool DeviceHasLeakyVDA(void)
{
    static int hasLeakyVDA = -1;
#if defined(TARGET_DARWIN_OSX)
    if (hasLeakyVDA == -1)
        hasLeakyVDA = NSAppKitVersionNumber <= NSAppKitVersionNumber10_9 ? 1 : 0;
#endif
    return hasLeakyVDA == 1;
}

const char* GetOSVersionString(void)
{
    CCocoaAutoPool pool;
    return [[[NSProcessInfo processInfo] operatingSystemVersionString] UTF8String];
}

float GetIOSVersion(void)
{
    CCocoaAutoPool pool;
    float version;
#if defined(TARGET_DARWIN_IOS)
    version = [[[UIDevice currentDevice] systemVersion] floatValue];
#else
    version = 0.0f;
#endif

    return (version);
}

const char* GetIOSVersionString(void)
{
#if defined(TARGET_DARWIN_IOS)
    static std::string iOSVersionString;
    if (iOSVersionString.empty()) {
        CCocoaAutoPool pool;
        iOSVersionString.assign((const char*)[[[UIDevice currentDevice] systemVersion] UTF8String]);
    }
    return iOSVersionString.c_str();
#else
    return "0.0";
#endif
}

const char* GetOSXVersionString(void)
{
#if defined(TARGET_DARWIN_OSX)
    static std::string OSXVersionString;
    if (OSXVersionString.empty()) {
        CCocoaAutoPool pool;
        OSXVersionString.assign((const char*)[[[NSDictionary dictionaryWithContentsOfFile:
                                                                 @"/System/Library/CoreServices/SystemVersion.plist"] objectForKey:@"ProductVersion"] UTF8String]);
    }

    return OSXVersionString.c_str();
#else
    return "0.0";
#endif
}

int GetExecutablePath(char* path, uint32_t* pathsize)
{
    CCocoaAutoPool pool;
    // see if we can figure out who we are
    NSString* pathname;

    // 1) Kodi application running under IOS
    // 2) Kodi application running under OSX
    pathname = [[NSBundle mainBundle] executablePath];
    strcpy(path, [pathname UTF8String]);
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinExecutablePath(b/c) -> %s", path);

    return 0;
}

const char* GetAppRootFolder(void)
{
    static std::string rootFolder = "";
    if (rootFolder.length() == 0) {
        if (IsIosSandboxed()) {
            // when we are sandbox make documents our root
            // so that user can access everything he needs
            // via itunes sharing
            rootFolder = "Documents";
        } else {
            rootFolder = "Library/Preferences";
        }
    }
    return rootFolder.c_str();
}

bool IsIosSandboxed(void)
{
    static int ret = -1;
    if (ret == -1) {
        uint32_t path_size = 2 * MAXPATHLEN;
        char given_path[2 * MAXPATHLEN];
        int result = -1;
        ret = 0;
        memset(given_path, 0x0, path_size);
        /* Get Application directory */
        result = GetExecutablePath(given_path, &path_size);
        if (result == 0) {
            // we re sandboxed if we are installed in /var/mobile/Applications
            if (strlen("/var/mobile/Applications/") < path_size && strncmp(given_path, "/var/mobile/Applications/", strlen("/var/mobile/Applications/")) == 0) {
                ret = 1;
            }

            // since ios8 the sandbox filesystem has moved to container approach
            // we are also sandboxed if this is our bundle path
            if (strlen("/var/mobile/Containers/Bundle/") < path_size && strncmp(given_path, "/var/mobile/Containers/Bundle/", strlen("/var/mobile/Containers/Bundle/")) == 0) {
                ret = 1;
            }
        }
    }
    return ret == 1;
}

int BatteryLevel(void)
{
    float batteryLevel = 0;
#if defined(TARGET_DARWIN_IOS)
    batteryLevel = [[UIDevice currentDevice] batteryLevel];
#else
    CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
    CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);

    CFDictionaryRef powerSource = NULL;
    const void* powerSourceVal;

    for (int i = 0; i < CFArrayGetCount(powerSources); i++) {
        powerSource = IOPSGetPowerSourceDescription(powerSourceInfo, CFArrayGetValueAtIndex(powerSources, i));
        if (!powerSource)
            break;

        powerSourceVal = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSNameKey));

        int curLevel = 0;
        int maxLevel = 0;

        powerSourceVal = CFDictionaryGetValue(powerSource, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue((CFNumberRef)powerSourceVal, kCFNumberSInt32Type, &curLevel);

        powerSourceVal = CFDictionaryGetValue(powerSource, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue((CFNumberRef)powerSourceVal, kCFNumberSInt32Type, &maxLevel);

        batteryLevel = (double)curLevel / (double)maxLevel;
    }
    CFRelease(powerSources);
    CFRelease(powerSourceInfo);
#endif
    return batteryLevel * 100;
}

bool CFStringRefToStringWithEncoding(CFStringRef source, std::string& destination, CFStringEncoding encoding)
{
    const char* cstr = CFStringGetCStringPtr(source, encoding);
    if (!cstr) {
        CFIndex strLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(source) + 1,
            encoding);
        char* allocStr = (char*)malloc(strLen);

        if (!allocStr)
            return false;

        if (!CFStringGetCString(source, allocStr, strLen, encoding)) {
            free((void*)allocStr);
            return false;
        }

        destination = allocStr;
        free((void*)allocStr);

        return true;
    }

    destination = cstr;
    return true;
}

void PrintDebugString(std::string debugString)
{
    NSLog(@"Debug Print: %s", debugString.c_str());
}

bool CFStringRefToString(CFStringRef source, std::string& destination)
{
    return CFStringRefToStringWithEncoding(source, destination, CFStringGetSystemEncoding());
}

bool CFStringRefToUTF8String(CFStringRef source, std::string& destination)
{
    return CFStringRefToStringWithEncoding(source, destination, kCFStringEncodingUTF8);
}

const std::string& GetManufacturer(void)
{
    static std::string manufName;
    if (manufName.empty()) {
#ifdef TARGET_DARWIN_IOS
        // to avoid dlloading of IOIKit, hardcode return value
        // until other than Apple devices with iOS will be released
        manufName = "Apple Inc.";
#elif defined(TARGET_DARWIN_OSX)
        const CFMutableDictionaryRef matchExpDev = IOServiceMatching("IOPlatformExpertDevice");
        if (matchExpDev) {
            const io_service_t servExpDev = IOServiceGetMatchingService(kIOMasterPortDefault, matchExpDev);
            if (servExpDev) {
                CFTypeRef manufacturer = IORegistryEntryCreateCFProperty(servExpDev, CFSTR("manufacturer"), kCFAllocatorDefault, 0);
                if (manufacturer) {
                    if (CFGetTypeID(manufacturer) == CFStringGetTypeID())
                        manufName = (const char*)[[NSString stringWithString:(NSString*)manufacturer] UTF8String];
                    else if (CFGetTypeID(manufacturer) == CFDataGetTypeID()) {
                        manufName.assign((const char*)CFDataGetBytePtr((CFDataRef)manufacturer), CFDataGetLength((CFDataRef)manufacturer));
                        if (!manufName.empty() && manufName[manufName.length() - 1] == 0)
                            manufName.erase(manufName.length() - 1); // remove extra null at the end if any
                    }
                    CFRelease(manufacturer);
                }
            }
            IOObjectRelease(servExpDev);
        }
#endif // TARGET_DARWIN_OSX
    }
    return manufName;
}

bool IsAliasShortcut(const std::string& path)
{
    bool ret = false;
#if defined(TARGET_DARWIN_OSX)
    NSString* nsPath = [NSString stringWithUTF8String:path.c_str()];
    NSURL* nsUrl = [NSURL fileURLWithPath:nsPath];
    NSNumber* wasAliased = nil;

    if (nsUrl != nil) {
        NSError* error = nil;

        if ([nsUrl getResourceValue:&wasAliased forKey:NSURLIsAliasFileKey error:&error]) {
            ret = [wasAliased boolValue];
        }
    }
#endif
    return ret;
}

void TranslateAliasShortcut(std::string& path)
{
#if defined(TARGET_DARWIN_OSX)
    NSString* nsPath = [NSString stringWithUTF8String:path.c_str()];
    NSURL* nsUrl = [NSURL fileURLWithPath:nsPath];

    if (nsUrl != nil) {
        NSError* error = nil;
        NSData* bookmarkData = [NSURL bookmarkDataWithContentsOfURL:nsUrl error:&error];
        if (bookmarkData) {
            BOOL isStale = NO;
            NSURLBookmarkResolutionOptions options = NSURLBookmarkResolutionWithoutUI | NSURLBookmarkResolutionWithoutMounting;

            NSURL* resolvedURL = [NSURL URLByResolvingBookmarkData:bookmarkData
                                                           options:options
                                                     relativeToURL:nil
                                               bookmarkDataIsStale:&isStale
                                                             error:&error];
            if (resolvedURL) {
                // [resolvedURL path] returns a path as /dir/dir/file ...
                path = (const char*)[[resolvedURL path] UTF8String];
            }
        }
    }
#endif
}

bool CreateAliasShortcut(const std::string& fromPath, const std::string& toPath)
{
    bool ret = false;
#if defined(TARGET_DARWIN_OSX)
    NSString* nsToPath = [NSString stringWithUTF8String:toPath.c_str()];
    NSURL* toUrl = [NSURL fileURLWithPath:nsToPath];
    NSString* nsFromPath = [NSString stringWithUTF8String:fromPath.c_str()];
    NSURL* fromUrl = [NSURL fileURLWithPath:nsFromPath];
    NSError* error = nil;
    NSData* bookmarkData = [toUrl bookmarkDataWithOptions:NSURLBookmarkCreationSuitableForBookmarkFile includingResourceValuesForKeys:nil relativeToURL:nil error:&error];

    if (bookmarkData != nil && fromUrl != nil && toUrl != nil) {
        if ([NSURL writeBookmarkData:bookmarkData toURL:fromUrl options:NSURLBookmarkCreationSuitableForBookmarkFile error:&error]) {
            ret = true;
        }
    }
#endif
    return ret;
}

#endif

const char* GetSystemMachineName()
{
    CCocoaAutoPool pool;
    CFStringRef _name = SCDynamicStoreCopyComputerName(NULL, NULL);
    return [(NSString*)_name UTF8String];
}

const char* GetDeviceModel()
{
    CCocoaAutoPool pool;
    NSString* deviceModel = [AppleDevice currentDevice].deviceModel;
    return [(NSString*)deviceModel UTF8String];
}

const char* GetDefaultBrowser()
{
    CCocoaAutoPool pool;
    NSString* handler = (__bridge NSString*)LSCopyDefaultHandlerForURLScheme(
        (__bridge CFStringRef) @"http");
    NSString* appName = [[handler componentsSeparatedByString:@"."] lastObject];
    return [(NSString*)appName UTF8String];
}
