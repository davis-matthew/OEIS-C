/*
 * A356339  a(n) = Sum_{k=1..n} binomial(2*n, n-k) * sigma_2(k).
 * https://oeis.org/A356339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, n-k) * sigma_2(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* accumulator for the sum   */
    seq_t binom = 1;          /* binomial(2*n, 0)          */

    for (idx_t k = n; k > 0; --k) {
        /* sigma_2(k) = sigma_k(k, 2) */
        seq_t sigma = sigma_k(k, 2u);

        sum += binom * sigma; /* add current term */

        /* Prepare binomial for the next (k-1) when k>1 */
        if (k > 1) {
            seq_t num = (seq_t)(n + k);
            seq_t den = (seq_t)(n - k + 1);
            binom = (binom * num) / den;   /* exact division */
        }
    }

    return sum;
}
