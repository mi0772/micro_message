/**
 * Project: micro_message
 * Author: Carlo Di Giuseppe
 * Date: 20/06/25
 * License: MIT
 */
#include <stdio.h>
#include <string.h>
#include "string_man.h"

void to_lower(char *str) {
    if (str == NULL) return; // Check for NULL pointer
    for (char *p = str; *p; p++) {
        if (*p >= 'A' && *p <= 'Z') *p += ('a' - 'A');
    }
}
