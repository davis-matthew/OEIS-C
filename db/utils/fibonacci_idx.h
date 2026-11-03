/* fibonacci_idx: the Fibonacci number F(n), F(0) = 0.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FIBONACCI_IDX_H
#define OEISC_UTILS_FIBONACCI_IDX_H
#include <stddef.h>
#include <stdint.h>

/* small Fibonacci routine (e+1 ≤ 64 for n ≤ 10^4) */
static uint64_t fibonacci_idx(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t a = 0, b = 1, c;
    for (idx_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

#endif
