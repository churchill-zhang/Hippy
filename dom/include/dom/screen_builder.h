#pragma once

#include <functional>
#include <vector>
#include <mutex>

#include "dom/dom_manager.h"
#include "dom/dom_node.h"
#include "dom/screen.h"
#include "core/napi/js_native_api_types.h"

class Scope;

namespace hippy {
inline namespace dom {

class ScreenBuilder {
 public:
  ScreenBuilder() = default;
  ~ScreenBuilder() = default;

  void Create(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  void Update(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  void Delete(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  void AddEventListener(std::shared_ptr<Scope>& scope, size_t argument_count, const std::shared_ptr<hippy::napi::CtxValue> arguments[]);
  Screen Build(const std::weak_ptr<DomManager>& dom_manager);

 private:
  std::vector<std::function<void()>> ops_;
  std::mutex mutex_;
};

}  // namespace dom
}  // namespace hippy
