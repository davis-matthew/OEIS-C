/*
 * A386642  Roll n fair, distinguishable, n-sided dice and sort the dice in
 *          increasing order with the lowest on the left, highest on the
 *          right. Let m be the maximum value rolled and select the m-th die
 *          from the right. Then a(n) is the number of such rolls (over all
 *          n^n possible outcomes) where the selected d...
 * https://oeis.org/A386642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - sum_{k=0..n} binom(n,k)*(k-1)^k + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (n >= 1, n is the sequence index) */

#include "../utils/pow_seq.h"  /* pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* Use unsigned intermediate arithmetic; seq_t may be an arbitrary-precision type. */
    /* Compute n^n */
    seq_t pow_n_n = pow_seq((seq_t)n, (int)n);

    /* c holds binomial(n,k) iteratively (starting with k=0 -> c=1) */
    seq_t c = 1;
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute (k-1)^k.  For k==0 this is 1 by definition. */
        seq_t pow_k;
        if (k == 0)
            pow_k = 1;
        else
            pow_k = pow_seq((seq_t)(k - 1), (int)k);

        /* Add term c * pow_k to the sum. */
        sum += c * pow_k;

        /* Prepare binomial coefficient for next k (if any). */
        if (k < n) {
            /* binom(n, k+1) = binom(n, k) * (n - k) / (k + 1) */
            c = (c * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }

    /* a(n) = n^n - sum + 1 */
    seq_t result = pow_n_n - sum + 1;
    return result;
}
