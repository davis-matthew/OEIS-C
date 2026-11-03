/*
 * A340101  Number of factorizations of 2n + 1 into odd factors > 1.
 * https://oeis.org/A340101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7611, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 121 terms through n = 9968 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of factorizations of 2n+1 into odd factors > 1
   a(n) = A001055(2n+1)  (OEIS A340101) */
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

/* Recursive helper: count the number of unordered factorizations of m
   into factors >= min_factor. Each factor must be >1, and m is odd,
   so all divisors are automatically odd. */
static seq_t
count_factorizations(seq_t m, seq_t min_factor)
{
    if (m == 1)                      /* single factor (the number itself) */
        return 1;

    seq_t count = 1;                 /* representation with factor m */
    seq_t limit = floor_sqrt(m);

    for (seq_t d = min_factor; d <= limit; ++d) {
        if (m % d == 0) {
            count += count_factorizations(m / d, d);
        }
    }
    return count;
}

/* Public interface: a(n) for OEIS A340101 */
seq_t
a(idx_t n)
{
    seq_t m = (seq_t)n * 2 + 1;       /* 2n + 1 is always odd */
    return count_factorizations(m, 2);
}
