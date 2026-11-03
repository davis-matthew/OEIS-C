/*
 * A292560  Expansion of Product_{k>=1} 1/(1 + x^(k^3)).
 * https://oeis.org/A292560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1619, i16 n <= 8880, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9617 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A292560:  a(n) = coefficient of ∏_{k≥1} 1/(1 + x^(k³)).
    Recurrence:  a(0)=1;
    For n>0, a(n) = –∑_{k: k³≤n} a(n−k³).  */

/*  Necessary utilities and headers  */
#include <stdlib.h>                /*  calloc, free  */
#include <stdint.h>
#include <limits.h>
#include "../utils/mem.h"

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Allocate dp[0…n] and zero‑initialize it. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) return 0;              /*  safety fallback  */
    dp[0] = 1;

    /* Process all cubes m = k³ ≤ n.  */
    idx_t maxk = int_cube_root(n);
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t m = k * k * k;
        for (idx_t s = m; s <= n; ++s) {
            dp[s] -= dp[s - m];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
