/*
 * A369078  Expansion of (1/x) * Series_Reversion( x * (1+x^2/(1-x))^3 ).
 * https://oeis.org/A369078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * sum_{k=0..floor(n/2)} (-1)^k * binomial(3*n +
 *            k + 2, k) * binomial(n - k - 1, n - 2*k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: binomial(idx_t, idx_t) is provided in the utils library. */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* handle the offset n=0 explicitly */
    if (n == 0) {
        return 1;
    }

    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* first binomial: C(3*n + k + 2, k) */
        idx_t arg1_n = 3 * n + k + 2;
        seq_t term1 = binomial(arg1_n, k);

        /* second binomial: C(n - k - 1, n - 2*k) */
        idx_t arg2_n = n - k - 1;
        idx_t arg2_k = n - 2 * k;
        seq_t term2 = binomial(arg2_n, arg2_k);

        /* product term */
        seq_t prod = term1 * term2;

        /* add or subtract depending on the parity of k */
        if (k & 1) {
            sum -= prod;
        } else {
            sum += prod;
        }
    }

    /* divide by (n+1), the result is guaranteed to be integral */
    sum /= (seq_t)(n + 1);
    return sum;
}
