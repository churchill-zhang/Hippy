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

#include "ios_loader_utils.h"

NSURL *IOSLoaderUtil::CreateURLFromUnicodeString(const unicode_string_view &unicodeString) {
    unicode_string_view utf8String = hippy::base::StringViewUtils::Convert(unicodeString, unicode_string_view::Encoding::Utf8);
    unicode_string_view::u8string string = utf8String.utf8_value();
    size_t length = string.length();
    if (length) {
        const unsigned char *c_str = string.c_str();
        CFURLRef URLRef = CFURLCreateWithBytes(NULL, c_str, length, kCFStringEncodingUTF8, NULL);
        return CFBridgingRelease(URLRef);
    }
    return nil;
}

IOSLoaderUtil::unicode_string_view IOSLoaderUtil::ConvertCStringToUnicode16String(const char *c_str) {
    unicode_string_view stringView = hippy::base::StringViewUtils::ConstCharPointerToStrView(c_str);
    unicode_string_view u16StringView = hippy::base::StringViewUtils::Convert(stringView, tdf::base::unicode_string_view::Encoding::Utf16);
    return u16StringView;
}

IOSLoaderUtil::RetCode IOSLoaderUtil::ConvertURIErrorToCode(NSError *error) {
    RetCode retCode = RetCode::Failed;
    if (error) {
        NSInteger code = [error code];
        switch (code) {
            case kCFURLErrorBadURL:
            case kCFURLErrorUnsupportedURL:
                retCode = RetCode::UriError;
                break;
            case kCFURLErrorResourceUnavailable:
            case kCFURLErrorZeroByteResource:
                retCode = RetCode::ResourceNotFound;
                break;
            case kCFURLErrorTimedOut:
                retCode = RetCode::Timeout;
                break;
            default:
                break;
        }
    }
    return retCode;
}

IOSLoaderUtil::RetCode IOSLoaderUtil::ConvertFileErrorToCode(NSError *error) {
    RetCode retCode = RetCode::Failed;
    if (error) {
        NSInteger code = [error code];
        switch (code) {
            case NSFileNoSuchFileError:
            case NSFileReadNoSuchFileError:
                retCode = RetCode::ResourceNotFound;
                break;
            default:
                break;
        }
    }
    return retCode;
}
