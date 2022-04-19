#include "dom/screen_builder.h"

#include "base/logging.h"
#include "core/base/string_view_utils.h"
#include "core/scope.h"

const uint32_t kInvalidListenerId = 0;

namespace hippy {
inline namespace dom {

void ScreenBuilder::Create(const std::weak_ptr<DomManager>& dom_manager,
                           std::vector<std::shared_ptr<DomNode>>&& nodes) {
  std::lock_guard<std::mutex> lock(mutex_);

  ops_.emplace_back([dom_manager, move_nodes = std::move(nodes)]() mutable {
    auto manager = dom_manager.lock();
    if (manager) {
      manager->CreateDomNodes(std::move(move_nodes));
    }
  });
}

void ScreenBuilder::Update(const std::weak_ptr<DomManager>& dom_manager,
                           std::vector<std::shared_ptr<DomNode>>&& nodes) {
  std::lock_guard<std::mutex> lock(mutex_);

  ops_.emplace_back([dom_manager, move_nodes = std::move(nodes)]() mutable {
    auto manager = dom_manager.lock();
    if (manager) {
      manager->UpdateDomNodes(std::move(move_nodes));
    }
  });
}

void ScreenBuilder::Delete(const std::weak_ptr<DomManager>& dom_manager,
                           std::vector<std::shared_ptr<DomNode>>&& nodes) {
  std::lock_guard<std::mutex> lock(mutex_);

  ops_.emplace_back([dom_manager, move_nodes = std::move(nodes)]() mutable {
    auto manager = dom_manager.lock();
    if (manager) {
      manager->DeleteDomNodes(std::move(move_nodes));
    }
  });
}

void ScreenBuilder::AddEventListener(std::shared_ptr<Scope>& scope, size_t argument_count,
                                     const std::shared_ptr<hippy::napi::CtxValue> arguments[]) {
  std::shared_ptr<hippy::napi::Ctx> context = scope->GetContext();
  TDF_BASE_CHECK(argument_count == 3) << "add event invalid parameter size";

  // get dom node id
  int32_t dom_id;
  bool flag = context->GetValueNumber(arguments[0], &dom_id);
  TDF_BASE_DCHECK(flag) << "get dom id failed";

  // get event name
  tdf::base::unicode_string_view str_view;
  flag = context->GetValueString(arguments[1], &str_view);
  std::string name = hippy::base::StringViewUtils::ToU8StdStr(str_view);
  TDF_BASE_DCHECK(flag) << "get event name failed";

  // check callback function
  TDF_BASE_DCHECK(context->IsFunction(arguments[2])) << "get event callback failed";
  auto callback = arguments[2];

  ops_.emplace_back([scope, dom_id, name, callback]() mutable {
    std::shared_ptr<hippy::napi::Ctx> context = scope->GetContext();
    auto weak_dom_manager = scope->GetDomManager();
    auto manager = weak_dom_manager.lock();
    if (!manager) {
      // TODO 异常处理
      return;
    }

    auto listener_id = scope->GetListenerId(static_cast<uint32_t>(dom_id), name);
    if (listener_id != kInvalidListenerId) {
      // TODO: 目前hippy上层还不支持绑定多个回调，有更新时先移除老的监听，再绑定新的
      manager->RemoveEventListener(static_cast<uint32_t>(dom_id), name, listener_id);
    }
    TDF_BASE_DLOG(INFO) << "UIManagerModule::AddEventListener id = " << dom_id;
    std::weak_ptr<hippy::napi::Ctx> weak_context = context;
    std::weak_ptr<Scope> weak_scope = scope;
    //  auto callback = arguments[2];
    manager->AddEventListener(
        static_cast<uint32_t>(dom_id), name, true,
        [weak_context, weak_scope, callback](std::shared_ptr<DomEvent>& event) {
          auto context = weak_context.lock();
          if (!context) {
            return;  // TODO
          }

          auto scope = weak_scope.lock();
          if (!scope) {
            return;  // TODO
          }
          // TODO: 先写在这里，后面调整代码位置
          context->RegisterDomEvent(scope, callback, event);
        },
        [weak_scope, dom_id, name](const std::shared_ptr<DomArgument>& arg) {
          tdf::base::DomValue dom_value;
          std::shared_ptr<Scope> scope = weak_scope.lock();
          if (scope && arg->ToObject(dom_value) && dom_value.IsUInt32()) {
            scope->AddListener(static_cast<uint32_t>(dom_id), name, dom_value.ToUint32Checked());
          }
        });
  });
}

Screen ScreenBuilder::Build(const std::weak_ptr<DomManager>& dom_manager) {
  std::lock_guard<std::mutex> lock(mutex_);

  ops_.emplace_back([dom_manager]{
    auto manager = dom_manager.lock();
    if (manager) {
      manager->EndBatch();
    }
  });
  return Screen(std::move(ops_));
}

}  // namespace dom
}  // namespace hippy
