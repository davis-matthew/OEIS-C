/*
 * A357408  a(n) is the least sum n + y such that 1/n + 1/y = 1/z with
 *          gcd(n,y,z) = 1, for some integers y and z.
 * https://oeis.org/A357408
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{ t > n | t divides n^2 and gcd(n, t-n, n - n^2/t) = 1
 *            }
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the smallest divisor t of n² exceeding n that satisfies
   the co‑primeness condition.  It works for any seq_t and idx_t supported by the
   build, and runs in O(n) time for each call. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Integer floor square‑root for unsigned 64‑bit values. */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid == 0) { lo = mid; continue; }
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* undefined for indices below the offset */

    idx_t sqrt_n = isqrt_u64(n);  /* floor(sqrt(n)) */
    idx_t m;
    for (m = sqrt_n + 1; m <= n; ++m) {
        if (n % m != 0) continue; /* m must divide n */

        seq_t t = (seq_t)m * (seq_t)m;        /* candidate sum n+y */
        seq_t n2 = (seq_t)n * (seq_t)n;       /* n² */
        seq_t s = n2 / t;                     /* n² / t, integer division */
        seq_t z = (seq_t)n - s;               /* z = n - n²/t */
        seq_t y = t - (seq_t)n;               /* y = t - n */

        seq_t g1 = gcd((seq_t)n, y);
        seq_t g  = gcd(g1, z);
        if (g == (seq_t)1) return t;          /* minimal valid t found */
    }

    /* Fallback: the trivial solution t = n² always works. */
    return (seq_t)n * (seq_t)n;
}
