#pragma once

#include <functional>
#include <vector>

#include "dom/dom_manager.h"
#include "dom/dom_node.h"
#include "dom/screen.h"

namespace hippy {
inline namespace dom {

class ScreenBuilder {
 public:
  ScreenBuilder() = default;
  ~ScreenBuilder() = default;

  void Create(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  void Update(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  void Delete(const std::weak_ptr<DomManager>& dom_manager, std::vector<std::shared_ptr<DomNode>>&& nodes);
  Screen Build();
 private:
  std::vector<std::function<void()>> ops_;
};

}
}
