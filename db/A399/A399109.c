/*
 * A399109  Sum of the cubes of the coefficients of (x + y + 2*z)^n.
 * https://oeis.org/A399109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i+j+k=n} (n!/(i! j! k!))^3 * 8^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* compute a(n) */
seq_t a(idx_t n) {
    /* handle n == 0 separately (quick path) */
    if (n == 0) return (seq_t)1;

    /* allocate arrays for binomial coefficients of n and powers of 8 */
    seq_t *binom = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow8  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!binom || !pow8) {
        /* allocation failed – return 0 */
        if (binom) free(binom);
        if (pow8)  free(pow8);
        return (seq_t)0;
    }

    /* binomial[n][i] = C(n,i) */
    binom[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* binom[i] = binom[i-1] * (n-i+1) / i */
        seq_t num = binom[i - 1] * (seq_t)(n - i + 1);
        binom[i] = num / (seq_t)i;
    }

    /* powers of 8 */
    pow8[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        pow8[i] = pow8[i - 1] * (seq_t)8;
    }

    seq_t result = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t binom_i = binom[i];
        seq_t binom_i_cubed = binom_i * binom_i * binom_i;

        idx_t max_j = n - i;
        seq_t binom_j = (seq_t)1;       /* C(n-i, 0) = 1 */
        seq_t binom_j_cubed = (seq_t)1; /* its cube */

        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t k = n - i - j;
            /* term = binom_i_cubed * binom_j_cubed * 8^k */
            seq_t term = binom_i_cubed * binom_j_cubed;
            term = term * pow8[k];
            result += term;

            if (j < max_j) {
                /* Update binom_j for next j:
                   C(n-i, j+1) = C(n-i, j) * (n-i-j)/(j+1) */
                seq_t mul = (seq_t)(n - i - j);   /* numerator */
                seq_t div = (seq_t)(j + 1);       /* denominator */
                binom_j = binom_j * mul / div;
                binom_j_cubed = binom_j * binom_j * binom_j;
            }
        }
    }

    free(binom);
    free(pow8);
    return result;
}
