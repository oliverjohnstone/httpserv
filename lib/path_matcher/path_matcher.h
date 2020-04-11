//
// Created by Oliver Johnstone on 07/04/2020.
//

#ifndef PATH_MATCHER_PATH_MATCHER_H
#define PATH_MATCHER_PATH_MATCHER_H

#include <string>
#include <unordered_map>

namespace PathMatcher {
    typedef std::unordered_map<std::string, std::string> ArgResults;

    class Matcher {
    private:
        class Implementation;
        Implementation *implementation;

    public:
        explicit Matcher(const std::string& pathToMatch);
        virtual ~Matcher();
        bool matches(const std::string& path, ArgResults& argsOut);
    };
}

#endif //PATH_MATCHER_PATH_MATCHER_H
