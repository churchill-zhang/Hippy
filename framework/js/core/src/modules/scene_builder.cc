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

#include "core/modules/scene_bulider.h"

#include "core/scope.h"
#include "base/unicode_string_view.h"
#include "core/napi/js_native_api_types.h"
#include "core/modules/ui_manager_module.h"
#include "dom/scene_builder.h"

template <typename T>
using InstanceDefine = hippy::napi::InstanceDefine<T>;

template <typename T>
using FunctionDefine = hippy::napi::FunctionDefine<T>;



using CtxValue = hippy::napi::CtxValue;

namespace hippy {

std::shared_ptr<InstanceDefine<SceneBuilder>> RegisterSceneBuilder(const std::weak_ptr<Scope>& weak_scope) {
  using SceneBuilder = hippy::dom::SceneBuilder;
  InstanceDefine<SceneBuilder> def;
  def.name = "SceneBuilder";
  def.constructor = [](size_t argument_count,
                       const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<SceneBuilder> {
    return std::make_shared<SceneBuilder>();
  };

  FunctionDefine<SceneBuilder> create_func_def;
  create_func_def.name = "Create";
  create_func_def.cb = [weak_scope](
      SceneBuilder* builder,
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

  FunctionDefine<SceneBuilder> update_func_def;
  update_func_def.name = "Update";
  update_func_def.cb = [weak_scope](
      SceneBuilder* builder,
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

  FunctionDefine<SceneBuilder> delete_func_def;
  delete_func_def.name = "Delete";
  delete_func_def.cb = [weak_scope](
      SceneBuilder* builder,
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

  FunctionDefine<SceneBuilder> add_event_listener_def;
  add_event_listener_def.name = "AddEventListener";
  add_event_listener_def.cb = [weak_scope](
      SceneBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    auto scope = weak_scope.lock();
    if (scope) {
      builder->AddEventListener(scope, argument_count, arguments);
    }
    return nullptr;
  };
  def.functions.emplace_back(std::move(add_event_listener_def));

  // TODO remove event listener
  FunctionDefine<SceneBuilder> remove_event_listener_def;
  remove_event_listener_def.name = "RemoveEventListener";
  remove_event_listener_def.cb = [weak_scope](
      SceneBuilder* builder,
      size_t argument_count,
      const std::shared_ptr<CtxValue> arguments[]) -> std::shared_ptr<CtxValue> {
    return nullptr;
  };
  def.functions.emplace_back(std::move(remove_event_listener_def));


  FunctionDefine<SceneBuilder> build_func_def;
  build_func_def.name = "Build";
  build_func_def.cb = [weak_scope](
      SceneBuilder* builder,
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

  std::shared_ptr<InstanceDefine<SceneBuilder>> build = std::make_shared<
      InstanceDefine<SceneBuilder>>(def);
  auto scope = weak_scope.lock();
  if (scope) {
    scope->SaveClassInstance(build);
  }

  return build;
}

}


