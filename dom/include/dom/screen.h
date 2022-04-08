#pragma once

#include <functional>
#include <vector>

namespace hippy {
inline namespace dom {

class Screen {
 public:
  Screen(std::vector<std::function<void()>>&& ops);
  ~Screen() = default;

  void Build();

 private:
  std::vector<std::function<void()>> ops_;
};

}
}
