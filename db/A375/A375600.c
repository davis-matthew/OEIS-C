/*
 * A375600  Triangle read by rows: T(n, k) = n! * 3^k * hypergeom([-k], [-n],
 *          2/3).
 * https://oeis.org/A375600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle rows of T(n,k) = n! * 3^k *
 *            hypergeom([-k],[-n],2/3) As integers: T(n,k) = Σ_{m=0..k} C(k,m)
 *            * (n-k+m)! * 3^m * 2^{k-m}
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Find (row r, column k) from flattened index n */
    idx_t N = n;
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)N) - 1.0) / 2.0);
    idx_t tri = r * (r + 1) / 2;
    idx_t k = N - tri;                /* 0 <= k <= r */

    /* Precompute factorials up to r */
    seq_t *fact = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!fact) return 0;              /* out of memory -> return 0 */
    fact[0] = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Precompute powers of 2 and 3 up to k */
    seq_t *pow2 = (seq_t*)malloc((size_t)(k + 1) * sizeof(seq_t));
    seq_t *pow3 = (seq_t*)malloc((size_t)(k + 1) * sizeof(seq_t));
    if (!pow2 || !pow3) {
        free(fact);
        free(pow2); free(pow3);
        return 0;
    }
    pow2[0] = 1; pow3[0] = 1;
    for (idx_t i = 1; i <= k; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }

    /* Compute the sum */
    seq_t sum = 0;
    for (idx_t m = 0; m <= k; ++m) {
        /* Binomial coefficient C(k, m) */
        seq_t bin = 1;
        for (idx_t i = 1; i <= m; ++i)
            bin = bin * (k - m + i) / i;     /* exact division */

        idx_t fact_index = r - k + m;          /* 0 <= fact_index <= r */
        seq_t term = bin * fact[fact_index];
        term = term * pow3[m];
        term = term * pow2[k - m];
        sum += term;
    }

    free(fact);
    free(pow2);
    free(pow3);
    return sum;
}
