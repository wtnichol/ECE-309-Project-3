// src/tools/workspace.cpp — YOU IMPLEMENT (spec §4.4).
//
// Workspace::resolve: a purely lexical check, steps 1–6 of the spec.
// Do not touch the filesystem here (no exists(), canonical(), ...).
// The constructor, root() and check_symlinks() are provided in
// workspace_provided.cpp.

#include "tools/workspace.h"

namespace fs = std::filesystem;

bool Workspace::resolve(std::string_view user_path, fs::path* out,
                        std::string* reason) const {
    if (user_path.size() == 0) {   //checks if the file path given is empty or not
        *reason = "empty path";
        return false;
    }
    for (std::size_t i = 0; i < user_path.size(); i++) {
        if (user_path[i] == '\0') {  //checks if the file path name contains a null character
            *reason = "cannot contain \0";
            return false;
        }
    }
    std::filesystem::path p = std::string(user_path);
    if ((p.is_absolute()) || (p.has_root_path())) {   //checks to see if the path has an absolute or root path
        *reason = "has a absolute/root path";
        return false;
    }
    std::filesystem::path n = p.lexically_normal();
    if (*n.begin() == "..") {   //checks to see if the path goes past the root
        *reason = "path climbs above root";
        return false;
    }
    if (n == ".") {
        *out = root();     //selects the root and accepted other roots into it
        return true;
    }
    else {
        *out = root() / n; //selects the root and accepted other roots into it
        return true;
    }
}
