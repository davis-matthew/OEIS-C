/*
 * A247785  Numbers k such that A247784(k+1) = A247784(k).
 * https://oeis.org/A247785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A247784(k+1) = A247784(k), where A247784(k) =
 *            floor((12k+11)/(6e)); found by binary search on the count of
 *            such k
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 965 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A247785_u128;

static A247785_u128 A247785_f(unsigned long long k)
{
    /* 1/e ~ 0.36787944117144232160 */
    const A247785_u128 C = (A247785_u128)3678794411714423216ULL; /* scaled by 1e19 */
    const A247785_u128 D = (A247785_u128)10000000000000000000ULL * 6;
    return ((A247785_u128)(12ULL * k + 11ULL) * C) / D;
}

/* number of k in [1, K] with f(k+1) == f(k) */
static unsigned long long A247785_count(unsigned long long K)
{
    A247785_u128 d = A247785_f(K + 1) - A247785_f(1);
    return K - (unsigned long long)d;
}

seq_t a(idx_t n)
{
    unsigned long long target = (unsigned long long)n;
    unsigned long long lo = 1, hi = 4ULL * target + 16ULL;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo) / 2;
        if (A247785_count(mid) >= target) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
