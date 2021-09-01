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

#include "uri_loader.h"
#include "core/core.h"

using Delegate = hippy::base::UriLoader::Delegate;
using SyncContext = hippy::base::UriLoader::SyncContext;
using ASyncContext = hippy::base::UriLoader::ASyncContext;
using unicode_string_view = tdf::base::unicode_string_view;
using u8string = unicode_string_view::u8string;
using RetCode = hippy::base::UriLoader::RetCode;

/**
 * A default loader for HTTP request
 */
class HTTPLoader : public Delegate {
public:
    HTTPLoader() = default;
    virtual ~HTTPLoader() = default;
    virtual void RequestUntrustedContent(
        SyncContext& ctx,
        std::function<void(SyncContext&)> next);
    virtual void RequestUntrustedContent(
        ASyncContext& ctx,
        std::function<void(ASyncContext&)> next);
};

/**
 * A default loader for File request
 */
class FileLoader : public Delegate {
public:
    FileLoader() = default;
    virtual ~FileLoader() = default;
    virtual void RequestUntrustedContent(
        SyncContext& ctx,
        std::function<void(SyncContext&)> next);
    virtual void RequestUntrustedContent(
        ASyncContext& ctx,
        std::function<void(ASyncContext&)> next);
private:
    /** get file data from path in pararm unicode_string_view
     * @param path file path
     * @param content if success, content is file content, otherwise content is error message
     * @return result of reading data
     */
    RetCode getFileData(const unicode_string_view &path, std::string &content);
};

/**
 * A default loader for Data request
 * such as data url below
 * data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAACiYAAAWHCAYAAADK6l
 */
class DataLoader : public Delegate {
public:
    DataLoader() = default;
    virtual ~DataLoader() = default;
    virtual void RequestUntrustedContent(
        SyncContext& ctx,
        std::function<void(SyncContext&)> next);
    virtual void RequestUntrustedContent(
        ASyncContext& ctx,
        std::function<void(ASyncContext&)> next);
private:
    /** get data from url in pararm unicode_string_view
     * @param path file path
     * @param content if success, content is file content, otherwise content is error message
     * @return result of reading data
     */
    RetCode getContentData(const unicode_string_view &path, std::string &content);
};
