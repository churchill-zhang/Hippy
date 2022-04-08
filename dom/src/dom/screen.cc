#include "dom/screen.h"
#include "../../include/dom/screen.h"

#include <utility>

namespace hippy {
inline namespace dom {

Screen::Screen(std::vector<std::function<void()>>&& ops): ops_(std::move(ops)) {}

void Screen::Build() {
  for (const auto& op: ops_) {
    op();
  }
}

}
}
