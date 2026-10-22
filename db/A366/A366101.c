/*
 * A366101  Expansion of (1/x) * Series_Reversion( x*(1+x-x^5)/(1+x) ).
 * https://oeis.org/A366101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/5⌋} (-1)^(n-k) * binomial(n+k,k)
 *            * binomial(n-4k-1,n-5k)
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 36, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 1;                      /* k=0 gives 1 only for n=0 */

    seq_t sum = 0;
    const idx_t k_max = n / 5;          /* floor(n/5) */

    for (idx_t k = 1; k <= k_max; ++k) {
        /* Compute binomial(n+k, k) and binomial(n-4k-1, n-5k) */
        seq_t c1 = binomial(n + k, k);
        seq_t c2 = binomial(n - 4 * k - 1, n - 5 * k);

        seq_t term = c1 * c2;

        /* Apply the sign (-1)^(n-k) */
        if ((n - k) & 1)
            sum -= term;
        else
            sum += term;
    }

    /* k=0 contributes 0 for n>=1, so sum is final numerator */
    return sum / ((seq_t)n + 1);
}
