#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) : _argc(0), _argv(nullptr) {
            vector<string> parts;
            parts.reserve(8);
            size_t i = 0, n = cmd.size();
            while (i < n) {
                while (i < n && cmd[i] == ' ') ++i;
                if (i >= n) break;
                size_t j = i;
                while (j < n && cmd[j] != ' ') ++j;
                if (j > i) parts.emplace_back(cmd.substr(i, j - i));
                i = j;
            }
            _argc = static_cast<int>(parts.size());
            if (_argc == 0) {
                _argv = nullptr;
                return;
            }
            _argv = new char*[_argc + 1];
            for (int k = 0; k < _argc; ++k) {
                const string& s = parts[k];
                char* buf = new char[s.size() + 1];
                std::memcpy(buf, s.c_str(), s.size() + 1);
                _argv[k] = buf;
            }
            _argv[_argc] = nullptr;
        }
        ~arguments() {
            if (_argv != nullptr) {
                for (int i = 0; i < _argc; ++i) {
                    delete[] _argv[i];
                }
                delete[] _argv;
                _argv = nullptr;
            }
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
