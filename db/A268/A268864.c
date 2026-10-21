/*
 * A268864  Number of integers of the form n*k whose digits sum up to n where
 *          0 <= k <= n^2.
 * https://oeis.org/A268864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k | 0 <= k <= n^2 , digit_sum(n*k) == n } for n <= 99;
 *            a(n) = 0 for n > 99
 * Range:     u8 n <= 7, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* For n > 99 the value is always 0 by the analysis in the OEIS entry. */
    if (n > 99u) {
        return 0;
    }

    seq_t count = 0;
    idx_t max_k = n * n;          /* n <= 99, so this fits in idx_t safely. */
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t prod = (seq_t)n * (seq_t)k;
        if (digit_sum(prod) == (seq_t)n) {
            ++count;
        }
    }
    return count;
}
