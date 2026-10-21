/*
 * A245106  a(n) = Sum_{k=0..n} binomial(n,k) * 2^((n-k)^2) * 3^(k^2).
 * https://oeis.org/A245106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^((n-k)^2) * 3^(k^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* provides seq_t pow2(idx_t exp); */
#include "../utils/pow3.h"       /* provides seq_t pow3(idx_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;                 /* binomial(n,0) */
    idx_t N = n;

    for (idx_t k = 0; k <= N; ++k) {
        if (k > 0) {
            /* Update binomial coefficient: bin = binomial(N, k) */
            bin = bin * (seq_t)(N - k + 1) / (seq_t)k;
        }

        idx_t exp2 = (N - k) * (N - k);   /* (n-k)^2 */
        idx_t exp3 = k * k;               /* k^2 */

        seq_t term = bin * pow2(exp2);
        term = term * pow3(exp3);
        sum += term;
    }

    return sum;
}
