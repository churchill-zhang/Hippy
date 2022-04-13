#include "core/scope.h"
#include "core/napi/js_native_api_types.h"

namespace hippy {

std::shared_ptr<hippy::napi::InstanceDefine<ScreenBuilder>>
RegisterScreenBuilder(const std::weak_ptr<Scope>& weak_scope);

}

