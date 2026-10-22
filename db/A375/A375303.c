/*
 * A375303  a(n) is the rank of row n of A130517 in a lexicographic
 *          permutation of [1, ..., n].
 * https://oeis.org/A375303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i = 1}^{ceil(n/2)} (n - 2i + 1) * (n - i)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    /* For n == 1 the result is 0, which is also given by the formula. */

    /* Length of the first (descending‑parity) block. */
    idx_t L0 = (n + 1ULL) / 2ULL;          /* ceil(n/2) */

    /* Compute (n-1)!; for n==1 the loop is skipped and fact == 1. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i) {
        fact *= (seq_t)i;
    }

    seq_t result   = 0;  /* accumulated rank */
    seq_t cur_fact = fact;          /* current (n-i)! to use for each term */

    for (idx_t i = 1; i <= L0; ++i) {
        seq_t smaller = (seq_t)n - (seq_t)(2 * i) + 1;  /* n - 2i + 1 */
        result += smaller * cur_fact;

        if (i < L0) {
            /* Prepare (n-(i+1))! for the next iteration: divide by (n-i). */
            cur_fact /= (n - i);
        }
    }

    return result;
}
