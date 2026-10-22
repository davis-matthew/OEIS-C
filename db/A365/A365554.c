/*
 * A365554  Number of increasing paths from the bottom to the top of the
 *          n-hypercube (as a graded poset) which first encounter a vector of
 *          isolated zeros at stage k, weighted by k.
 * https://oeis.org/A365554
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=⌊n/2⌋}^{n-1}
 *            k*(binomial(k+1,n-k)-binomial(k-1,n-k))*k!*(n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* k ranges from floor(n/2) to n-1 */
    idx_t k_start = n / 2;
    idx_t M = n - k_start;  /* current (n-k)! denominator */

    /* Compute k_start! */
    seq_t fac_k = 1;
    for (idx_t i = 2; i <= k_start; ++i)
        fac_k *= (seq_t)i;

    /* Compute (n-k_start)! */
    seq_t fac_s = 1;
    for (idx_t i = 2; i <= M; ++i)
        fac_s *= (seq_t)i;

    seq_t result = 0;
    for (idx_t k = k_start; k < n; ++k) {
        seq_t bin1 = binomial(k + 1, n - k);
        seq_t bin2 = 0;
        if (k >= 1)
            bin2 = binomial(k - 1, n - k);
        seq_t diff = bin1 - bin2;

        seq_t term = diff;
        term *= (seq_t)k;
        term *= fac_k;
        term *= fac_s;

        result += term;

        if (k + 1 < n) {
            fac_k *= (seq_t)(k + 1);   /* (k+1)! from k!   */
            fac_s /= (seq_t)M;         /* (n-(k+1))! from (n-k)!  */
            --M;
        }
    }
    return result;
}
