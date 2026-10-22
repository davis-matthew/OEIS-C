/*
 * A365815  a(n) is the least centered n-gonal prime, or -1 if none exists.
 * https://oeis.org/A365815
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k ≥ 1 such that 1 + n·k·(k−1)/2 is a prime, or –1
 *            if no such k exists
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return -1 safely. */
    if (n < 3)
        return -1;

    /* Arbitrary large bound for the search; all known instances
       with a(n) = –1 have no prime within this range, whereas
       all other n have a prime for very small k. */
    const idx_t MAX_K = 100000;

    /* Special case: n = 8 yields a perfect‑square sequence,
       so we can return –1 immediately. */
    if (n == 8)
        return -1;

    for (idx_t k = 1; k <= MAX_K; ++k) {
        /* Compute C(n,k) = 1 + n·k·(k−1)/2 using seq_t arithmetic. */
        seq_t part   = (seq_t)k * (seq_t)(k - 1);   /* k*(k-1) */
        seq_t prod   = (seq_t)n * part;             /* n*k*(k-1) */
        seq_t cent   = (prod / 2) + 1;              /* +1 */

        if (cent > 1 && is_prime_big(cent))
            return cent;
    }

    /* If no prime was found within the bound, report absence. */
    return -1;
}
