/*
 * A345876  a(n) = Sum_{k=0..n} binomial(2*n, n-k) * k^n.
 * https://oeis.org/A345876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, n-k) * k^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute a(n) as defined above.  The computation is done in O(n)
   time using a running binomial coefficient and repeated k^n
   evaluations by exponentiation by squaring.  No dynamic allocation
   is required, so the code works for both C and C++ builds. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = binomial(2 * n, n);   /* bin = C(2n, n) for k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t powk = big_pow((seq_t)k, (seq_t)n);   /* k^n */
        sum += bin * powk;

        if (k < n) {
            /* Update bin to C(2n, n-(k+1)):
               bin_new = bin * (n - k) / (n + k + 1) */
            seq_t num = (seq_t)(n - k);
            seq_t den = (seq_t)(n + k + 1);
            bin = bin * num / den;
        }
    }
    return sum;
}
