/*
 * A340101  Number of factorizations of 2n + 1 into odd factors > 1.
 * https://oeis.org/A340101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7611, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of factorizations of 2n+1 into odd factors > 1
   a(n) = A001055(2n+1)  (OEIS A340101) */
#include "../utils/floor_sqrt.h"

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
