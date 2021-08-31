#pragma once

/*
 *      Copyright (C) 2010-2015 Team Kodi
 *      http://kodi.tv
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
 *  along with Kodi; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <string>

// We forward declare CFStringRef in order to avoid
// pulling in tons of Objective-C headers.
struct __CFString;
typedef const struct __CFString * CFStringRef;


extern "C" {

const char *getIosPlatformString(void);
bool        IsMavericks(void);
bool        IsSnowLeopard(void);
bool        DeviceHasRetina(double &scale);
bool        DeviceHasLeakyVDA(void);
const char *GetOSVersionString(void);
float       GetIOSVersion(void);
const char *GetIOSVersionString(void);
const char *GetSystemMachineName(void);
const char *GetOSXVersionString(void);
int         GetExecutablePath(char* path, uint32_t *pathsize);
const char *GetAppRootFolder(void);
const char *GetDeviceModel();
bool        IsIosSandboxed(void);
int         BatteryLevel(void);
void        PrintDebugString(std::string debugString);
bool        CFStringRefToString(CFStringRef source, std::string& destination);
bool        CFStringRefToUTF8String(CFStringRef source, std::string& destination);
const std::string&  GetManufacturer(void);
bool        IsAliasShortcut(const std::string& path);
void        TranslateAliasShortcut(std::string& path);
bool        CreateAliasShortcut(const std::string& fromPath, const std::string& toPath);


}
