/*
 * A283233  2*A000201.
 * https://oeis.org/A283233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n*r), r = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9942 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute a(n) as defined by A283233.  The implementation uses only
   integer arithmetic: a(n) = 2*⌊n*φ⌋.  Using the identity
   ⌊n*φ⌋ = (n + ⌊n*√5⌋)/2 (integer division), we obtain
   a(n) = 2*⌊(n + ⌊n*√5⌋)/2⌋ = (n + ⌊n*√5⌋) - ((n + ⌊n*√5⌋) mod 2). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; safe guard */

    seq_t n_seq = (seq_t)n;          /* cast index to seq_t */
    seq_t prod  = n_seq * n_seq * 5; /* 5*n^2        */
    seq_t m     = floor_sqrt(prod);  /* ⌊n*√5⌋       */
    seq_t val   = n_seq + m;         /* n + ⌊n*√5⌋   */
    if ((val % 2) != 0) --val;       /* subtract 1 if odd */
    return val;
}
