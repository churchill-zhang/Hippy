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

#include "ios_basic_delegate.h"
#include "ios_loader_utils.h"
#import <Foundation/Foundation.h>

#pragma mark HTTPLoader
void HTTPLoader::RequestUntrustedContent(SyncContext& ctx,
                             std::function<void(SyncContext&)> next) {
    NSURL *url = IOSLoaderUtil::CreateURLFromUnicodeString(ctx.uri);
    if (!url) {
        ctx.ret_code = RetCode::UriError;
        return;
    }
    NSURLSession *session = [NSURLSession sharedSession];
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    [session dataTaskWithURL:url completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (data) {
            ctx.ret_code = RetCode::Success;
            const char *bytes = reinterpret_cast<const char *>([data bytes]);
            std::string URIData(bytes, [data length]);
            ctx.content = URIData;
        }
        else {
            ctx.ret_code = IOSLoaderUtil::ConvertURIErrorToCode(error);
            ctx.content = [[error localizedFailureReason] UTF8String];
        }
        dispatch_semaphore_signal(semaphore);
    }];
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
}

void HTTPLoader::RequestUntrustedContent(ASyncContext& ctx,
                                     std::function<void(ASyncContext&)> next) {
    NSURL *url = IOSLoaderUtil::CreateURLFromUnicodeString(ctx.uri);
    if (!url) {
        ctx.cb(RetCode::UriError, nullptr);
        return;
    }
    NSURLSession *session = [NSURLSession sharedSession];
    [session dataTaskWithURL:url completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (data) {
            hippy::base::UriLoader::RetCode ret_code = RetCode::Success;
            const char *bytes = reinterpret_cast<const char *>([data bytes]);
            std::string URIData(bytes, [data length]);
            ctx.cb(ret_code, URIData);
        }
        else {
            hippy::base::UriLoader::RetCode ret_code = IOSLoaderUtil::ConvertURIErrorToCode(error);
            ctx.cb(ret_code, [[error localizedFailureReason] UTF8String]);
        }
    }];
}

#pragma mark FileLoader

void FileLoader::RequestUntrustedContent(SyncContext& ctx,
                             std::function<void(SyncContext&)> next) {
    std::string content;
    ctx.ret_code = getFileData(ctx.uri, content);
    ctx.content = content;
}

void FileLoader::RequestUntrustedContent(ASyncContext& ctx,
                                     std::function<void(ASyncContext&)> next) {
    std::string content;
    RetCode code = getFileData(ctx.uri, content);
    if (ctx.cb) {
        ctx.cb(code, content);
    }
}

RetCode FileLoader::getFileData(const unicode_string_view &path, std::string &content) {
    NSURL *url = IOSLoaderUtil::CreateURLFromUnicodeString(path);
    if (!url) {
        return RetCode::UriError;
    }
    NSError *error = nil;
    NSData *data = [NSData dataWithContentsOfURL:url options:NSDataReadingMappedIfSafe error:&error];
    if (error) {
        content = [[error localizedFailureReason] UTF8String];
        return IOSLoaderUtil::ConvertFileErrorToCode(error);
    }
    else {
        const char *bytes = reinterpret_cast<const char *>([data bytes]);
        content = std::string(bytes, [data length]);
        return RetCode::Success;
    }
    return RetCode::Failed;
}

#pragma mark DataLoader

void DataLoader::RequestUntrustedContent(SyncContext& ctx,
                             std::function<void(SyncContext&)> next) {
    std::string content;
    ctx.ret_code = getContentData(ctx.uri, content);
    ctx.content = content;
}

void DataLoader::RequestUntrustedContent(ASyncContext& ctx,
                                     std::function<void(ASyncContext&)> next) {
    std::string content;
    RetCode code = getContentData(ctx.uri, content);
    if (ctx.cb) {
        ctx.cb(code, content);
    }
}

RetCode DataLoader::getContentData(const unicode_string_view &path, std::string &content) {
    NSURL *url = IOSLoaderUtil::CreateURLFromUnicodeString(path);
    if (!url) {
        return RetCode::UriError;
    }
    NSError *error = nil;
    NSData *data = [NSData dataWithContentsOfURL:url options:NSDataReadingMappedIfSafe error:&error];
    if (error) {
        content = [[error localizedFailureReason] UTF8String];
        return IOSLoaderUtil::ConvertFileErrorToCode(error);
    }
    else {
        const char *bytes = reinterpret_cast<const char *>([data bytes]);
        content = std::string(bytes, [data length]);
        return RetCode::Success;
    }
    return RetCode::Failed;
}

