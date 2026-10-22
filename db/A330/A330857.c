/*
 * A330857  Total sum of divisors of all the parts in the partitions of n into
 *          2 distinct parts.
 * https://oeis.org/A330857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Σ_{i=1}^{⌊(n-1)/2⌋} σ(i) + σ(n‑i) (equivalently a(n) =
 *            Σ_{i=1}^{n-1} σ(i) - [n even]·σ(n/2) ) where σ(k) is the sum of
 *            divisors of k (OEIS A000203)
 * Range:     u8 n <= 18, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the OEIS sequence A330857: total sum of divisors of all
 *  the parts in the partitions of n into 2 distinct parts.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset: the first two terms are 0 by definition */
    if (n <= 2) return 0;

    /* We only need σ(k) for 1 ≤ k ≤ n‑1. */
    idx_t limit = n - 1;

    /* Allocate zero‑initialized array for σ[0..limit]. */
    seq_t *sigma = (seq_t *)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!sigma) return 0;          /* allocation failure – unlikely for n ≤ 10000 */

    /* Sieve for the divisor sum function. */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t dval = (seq_t)d;
        for (idx_t m = d; m <= limit; m += d) {
            sigma[m] += dval;
        }
    }

    /* Prefix sum: Σ_{i=1}^{limit} σ(i). */
    seq_t total_sum = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        total_sum += sigma[i];
    }

    /* Subtract σ(n/2) if the partition must use distinct parts
     * (i.e. when n is even, (n/2, n/2) is not allowed). */
    if ((n & 1) == 0) {
        idx_t half = n >> 1;
        total_sum -= sigma[half];
    }

    free(sigma);
    return total_sum;
}
