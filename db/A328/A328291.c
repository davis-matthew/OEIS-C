/*
 * A328291  a(n) is the greatest k >= 0 such that n*k has distinct decimal
 *          digits.
 * https://oeis.org/A328291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the greatest k ≥ 0 such that n·k has distinct decimal
 *            digits. The implementation searches k downward from ⌊9876543210
 *            / n⌋ until a value produces a product with all distinct digits
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   distinct_digits128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/distinct_digits128.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(n) = 0 for n > 9876543210 (follows from the definition). */
    if (n == 0) return 0;                   /* unused, keep for safety */
    if (n > 9876543210ULL) return 0;

    /* a(100 * m) = 0 for any m, so return early for multiples of 100. */
    if ((n % 100ULL) == 0) return 0;

    /* Compute the theoretical upper bound for k. */
    unsigned long long max_k_ul = 9876543210ULL / (unsigned long long)n;
    seq_t k = (seq_t)max_k_ul;               /* starting candidate */

    /* Search downward until we find a product with distinct digits. */
    while (1) {
        unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)k;
        if (distinct_digits128(prod))
            return k;                       /* found the greatest k */
        if (k == 0)                         /* should never happen except for n % 100 == 0 */
            return 0;
        k--;                                /* try next smaller k */
    }
}
