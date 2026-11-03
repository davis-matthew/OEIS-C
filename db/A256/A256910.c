/*
 * A256910  Trace of the enhanced triangular-number representation of n.
 * https://oeis.org/A256910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(2)=2. For n>0, let T be the largest triangular number
 *            <= n. If T==n then a(n)=n, otherwise a(n)=a(n-T). (The sequence
 *            is the trace of the enhanced triangular-number representation.)
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: largest triangular number <= n.
   Computes k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2
   using a binary search on k.  The result is T_k. */
static idx_t largest_triangular_leq(idx_t n)
{
    idx_t lo = 0;
    /* hi set large enough (2^32 > sqrt(2*n) for any n <= 2^64-1). */
    idx_t hi = (idx_t)1 << 32;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 tri128 = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri128 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)((unsigned __int128)lo * (lo + 1) / 2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    for (;;) {
        if (n == 2) return 2;      /* special base case */
        idx_t t = largest_triangular_leq(n);
        if (t == n)               /* n itself is triangular */
            return (seq_t)n;
        n -= t;                    /* recurse on the remainder */
    }
}
