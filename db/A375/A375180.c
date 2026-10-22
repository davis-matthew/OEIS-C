/*
 * A375180  a(n) = Sum_{k = 0..n-1} (-1)^(n+k+1)*binomial(3*n, k)^3.
 * https://oeis.org/A375180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^(n+k+1) * binomial(3*n, k)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial zero case early */
    if (n == 0) {
        return (seq_t)0;
    }

    /* 3*n fits easily in idx_t for n up to 10^4 and larger as well. */
    idx_t N = 3 * n;

    seq_t pos = 0;   /* sum of terms with positive sign */
    seq_t neg = 0;   /* sum of terms with negative sign */

    /* Compute binomial(3*n, k) iteratively to avoid recomputation. */
    seq_t binom = 1; /* binomial(3*n, 0) */

    for (idx_t k = 0; k < n; ++k) {
        /* term = binom^3 */
        seq_t term = binom * binom;      /* binom^2 */
        term = term * binom;             /* binom^3 */

        /* Determine sign: (-1)^(n+k+1) */
        if (((n + k + 1) & 1) != 0) {
            neg += term;
        } else {
            pos += term;
        }

        /* Update binom to C(3*n, k+1) if we are not done with the loop. */
        if (k + 1 < n) {
            seq_t numer = binom * (N - k);   /* binom * (3*n - k) */
            binom = numer / (k + 1);        /* exact division */
        }
    }

    /* pos >= neg for all n, so the subtraction stays within bounds. */
    return pos - neg;
}
