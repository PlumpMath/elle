#pragma once

#include <string>
#include <unordered_map>

#ifdef environ // Prevent windows.h macros.
# undef environ
#endif

#include <elle/compiler.hh>

namespace elle
{
  namespace os ELLE_API
  {
    using Environ = std::unordered_map<std::string, std::string>;

    /// Retrieve a fresh copy of environ.
    Environ
    environ();

    /// Retrieve a fresh copy of environ keeping only the entries that
    /// validate the predicate.
    template <typename Pred>
    Environ
    environ(Pred pred)
    {
      auto res = environ();
      erase_if(res, [&pred](auto const& p) {
          return !pred(p.first, p.second);
        });
      return res;
    }

    std::string
    setenv(std::string const& key, std::string const& val, bool overwrite);

    /// Get a specific value from environment. If value is not found,
    /// KeyError Exception is thrown
    std::string
    getenv(std::string const& key);

    /// Get a specific value from environment. If value is not found,
    /// default_ is returned
    std::string
    getenv(std::string const& key, std::string const& default_);

    /// Get a specific value from environment. If value is not found,
    /// default_ is returned
    std::string
    getenv(std::string const& key, char const* default_);

    /// Get a specific value from environment. If value is not found,
    /// default_ is returned
    int
    getenv(std::string const& key, int default_);

    /// Get a specific value from environment. If value is not found,
    /// default_ is returned
    bool
    getenv(std::string const& key, bool default_);

    /// Whether a key is defined in environment.
    bool
    inenv(std::string const& key);

    void
    unsetenv(std::string const& key);
  }
}
