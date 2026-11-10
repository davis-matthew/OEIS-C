/*
 * A328940  Numbers k such that k divides A003754(k+1).
 * https://oeis.org/A328940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search: a(n) is the n-th k with k | A003754(k+1). Consecutive
 *            A003754 terms are generated in native integers, and x mod k is
 *            updated incrementally (quotient and remainder), so the search
 *            never divides
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* next number after x whose binary expansion has no two adjacent 0's */
static uint64_t A328940_next(uint64_t x) {
    uint64_t y = x + 1;
    int tz = 0;
    uint64_t t = y;
    while ((t & 1) == 0) {
        t >>= 1;
        tz++;
    }
    if (tz >= 2) {
        for (int j = tz - 2; j >= 0; j -= 2) {
            y |= ((uint64_t)1 << j);
        }
    }
    return y;
}

seq_t a(idx_t n) {
    static uint64_t found[128];
    static int nfound = 0;
    static uint64_t k = 0;   /* last k examined */
    static uint64_t x = 0;   /* A003754(k+1); A003754(1) = 0 */
    static int64_t q = 0;    /* floor(x / k) */
    static int64_t r = 0;    /* x - q*k */

    if (n == 0 || n > 128) return (seq_t)0;

    while ((idx_t)nfound < n) {
        k++;
        uint64_t xn = A328940_next(x);
        int64_t d = (int64_t)(xn - x);
        x = xn;
        r += d - q;
        while (r < 0) {
            q--;
            r += (int64_t)k;
        }
        while (r >= (int64_t)k) {
            q++;
            r -= (int64_t)k;
        }
        if (r == 0) {
            found[nfound++] = k;
        }
    }
    return (seq_t)found[n - 1];
}
