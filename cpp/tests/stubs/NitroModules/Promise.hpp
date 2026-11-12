#pragma once

// Test-build stand-in for <NitroModules/Promise.hpp>: a synchronous
// result/error holder without JSI or thread pools. Only the pieces
// HybridXlsxWorkbook (getBuffer) and the tests touch are provided.

#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace margelo::nitro {

template <typename TResult>
class Promise {
public:
  static std::shared_ptr<Promise> create() {
    return std::shared_ptr<Promise>(new Promise());
  }

  void resolve(const TResult& result) {
    std::lock_guard<std::mutex> lock(_mutex);
    _result = result;
  }

  void resolve(TResult&& result) {
    std::lock_guard<std::mutex> lock(_mutex);
    _result = std::move(result);
  }

  void reject(const std::exception_ptr& error) {
    std::lock_guard<std::mutex> lock(_mutex);
    _error = error;
  }

  [[nodiscard]] bool isResolved() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _result.has_value();
  }

  [[nodiscard]] bool isRejected() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _error != nullptr;
  }

  const TResult& getResult() const {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_result.has_value()) {
      throw std::runtime_error("Cannot get result when Promise is not yet resolved!");
    }
    return *_result;
  }

private:
  Promise() = default;
  mutable std::mutex _mutex;
  std::optional<TResult> _result;
  std::exception_ptr _error;
};

} // namespace margelo::nitro
