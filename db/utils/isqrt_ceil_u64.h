/* isqrt_ceil_u64: ceil(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_CEIL_U64_H
#define OEISC_UTILS_ISQRT_CEIL_U64_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Utility: integer sqrt (ceil) ------------------------------ */
static uint64_t isqrt_ceil_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t r = (uint64_t)sqrtl((long double)n);
    if (r * r < n) ++r;
    return r;
}

#endif
