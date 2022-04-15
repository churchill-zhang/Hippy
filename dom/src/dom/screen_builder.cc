#include "dom/screen_builder.h"

#include "base/logging.h"

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

}
}
