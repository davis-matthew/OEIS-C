/* omega: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_H
#define OEISC_UTILS_OMEGA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
static inline int omega(uint64_t n)
{
    int cnt = 0;
    if (n % 2 == 0) {
        ++cnt;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            ++cnt;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

#endif
