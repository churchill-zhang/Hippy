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

#include "core/modules/screen_bulider.h"

#include "core/scope.h"
#include "core/napi/js_native_api_types.h"
#include "core/modules/ui_manager_module.h"
#include "dom/screen_builder.h"

template <typename T>
using InstanceDefine = hippy::napi::InstanceDefine<T>;

template <typename T>
using FunctionDefine = hippy::napi::FunctionDefine<T>;



using CtxValue = hippy::napi::CtxValue;

namespace hippy {

std::shared_ptr<InstanceDefine<ScreenBuilder>> RegisterScreenBuilder(const std::weak_ptr<Scope>& weak_scope) {
  using ScreenBuilder = hippy::dom::ScreenBuilder;
  InstanceDefine<ScreenBuilder> def;
  def.name = "ScreenBuilder";
  def.constructor = [](size_t argument_count,
                       const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<ScreenBuilder> {
    return std::make_shared<ScreenBuilder>();
  };

  FunctionDefine<ScreenBuilder> create_func_def;
  create_func_def.name = "Create";
  create_func_def.cb = [weak_scope](
      ScreenBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    auto scope = weak_scope.lock();
    if (scope) {
      auto weak_dom_manager = scope->GetDomManager();
      auto ret = HandleJsValue(scope->GetContext(), arguments[0], scope);
      builder->Create(weak_dom_manager, std::move(std::get<2>(ret)));
    }
    return nullptr;
  };
  def.functions.emplace_back(std::move(create_func_def));

  FunctionDefine<ScreenBuilder> update_func_def;
  update_func_def.name = "Update";
  update_func_def.cb = [weak_scope](
      ScreenBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    auto scope = weak_scope.lock();
    if (scope) {
      auto weak_dom_manager = scope->GetDomManager();
      auto ret = HandleJsValue(scope->GetContext(), arguments[0], scope);
      builder->Update(weak_dom_manager, std::move(std::get<2>(ret)));
    }
    return nullptr;
  };

  FunctionDefine<ScreenBuilder> delete_func_def;
  delete_func_def.name = "Delete";
  delete_func_def.cb = [weak_scope](
      ScreenBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    auto scope = weak_scope.lock();
    if (scope) {
      auto weak_dom_manager = scope->GetDomManager();
      auto ret = HandleJsValue(scope->GetContext(), arguments[0], scope);
      builder->Delete(weak_dom_manager, std::move(std::get<2>(ret)));
    }
    return nullptr;
  };
  def.functions.emplace_back(std::move(delete_func_def));

  FunctionDefine<ScreenBuilder> build_func_def;
  build_func_def.name = "Build";
  build_func_def.cb = [weak_scope](
      ScreenBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    auto scope = weak_scope.lock();
    if (scope) {
      auto weak_dom_manager = scope->GetDomManager();
      auto screen = builder->Build(weak_dom_manager);
      auto dom_manager = weak_dom_manager.lock();
      if (dom_manager) {
        dom_manager->PostTask([screen]() {
          screen.Build();
        });
      }
    }
    return nullptr;
  };
  def.functions.emplace_back(std::move(build_func_def));

  std::shared_ptr<InstanceDefine<ScreenBuilder>> build = std::make_shared<
      InstanceDefine<ScreenBuilder>>(def);
  auto scope = weak_scope.lock();
  if (scope) {
    scope->SaveClassInstance(build);
  }

  return build;
}

}


