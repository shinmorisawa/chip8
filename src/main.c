#include "default.h"
#include <stdio.h>

Result get_result() {
    Error e = (Error) {.msg = "wow", .code = 0};

    Result r = (Result) {
        .code = 0,
        .ok = true,
        .err = e,
    };

    return r;
}

int main() {
    Result r = get_result();
    printf("%s\n", r.err.msg);
    return 0;
}
