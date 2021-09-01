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

#include "ios_loader.h"
#include "core/core.h"
#include "ios_loader_utils.h"
#include "ios_basic_delegate.h"

IOSLoader::IOSLoader(RequestUntrustedContentPtr loader, CFTypeRef userData): loader_(loader), userData_(CFRetain(userData)) {
    std::shared_ptr<HTTPLoader> httpLoader = std::make_shared<HTTPLoader>();
    RegisterUriDelegate(u"http", httpLoader);
    
    std::shared_ptr<FileLoader> fileLoader = std::make_shared<FileLoader>();
    RegisterUriDelegate(u"file", fileLoader);
    
}

IOSLoader::~IOSLoader() {
    CFRelease(userData_);
    userData_ = nullptr;
}

unicode_string_view IOSLoader::GetScheme(const unicode_string_view& uri) {
    NSURL *url = IOSLoaderUtil::CreateURLFromUnicodeString(uri);
    if (url) {
        NSString *scheme = [url scheme];
        if (scheme) {
            unicode_string_view u16StringView = IOSLoaderUtil::ConvertCStringToUnicode16String([scheme UTF8String]);
            return u16StringView;
        }
    }
    return u"";
}
