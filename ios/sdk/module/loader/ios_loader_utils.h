/*
 *
 * Tencent is pleased to support the open source community by making
 * Hippy available.
 *
 * Copyright (C) 2019 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "core/core.h"
#import <Foundation/Foundation.h>

class IOSLoaderUtil {
public:
    using unicode_string_view = tdf::base::unicode_string_view;
    using RetCode = hippy::base::UriLoader::RetCode;
    
    /**
     * convert unicode_string_view string object to NSURL object
     */
    static NSURL *CreateURLFromUnicodeString(const unicode_string_view &string);
    
    /**
     * convert c string to unicode_string_view string
     */
    static unicode_string_view ConvertCStringToUnicode16String(const char *c_str);
    
    /**
     * convert HTTP URL request error to RetCode
     */
    static RetCode ConvertURIErrorToCode(NSError *error);
    
    /**
     * convert File URL request error to RetCode
     */
    static RetCode ConvertFileErrorToCode(NSError *error);
};
