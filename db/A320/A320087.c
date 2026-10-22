/*
 * A320087  Number of primitive (=aperiodic) ternary words with length less
 *          than or equal to n which are earlier in lexicographic order than
 *          any other word derived by cyclic shifts of the alphabet.
 * https://oeis.org/A320087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} Sum_{d|j} 3^(d-1) * mu(j/d) (μ = Möbius)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   compute_mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_mobius.h"

/* Compute Möbius function values 0..limit into array mu[0..limit]      */

seq_t a(idx_t n)
{
    /* static caches for incremental evaluation                     */
    static idx_t cache_limit = 0;
    static seq_t *cache = NULL;      /* a[0..cache_limit] */
    static int   *mu    = NULL;      /* Möbius values 0..cache_limit */
    static seq_t *pow3  = NULL;      /* pow3[k] = 3^k for 0≤k≤cache_limit */

    if (n > cache_limit) {
        /* Free old caches if any                                         */
        free(cache);
        free(mu);
        free(pow3);

        /* Allocate new caches up to n                                   */
        cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        mu    = (int*)calloc((size_t)(n + 1), sizeof(int));
        pow3  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));

        /* Precompute Möbius values and powers of 3                       */
        compute_mobius(n, mu);
        pow3[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;

        /* Compute the sequence values incrementally                     */
        cache[0] = 0;
        for (idx_t j = 1; j <= n; ++j) {
            seq_t pos = 0, neg = 0;          /* positive and negative sums */
            for (idx_t d = 1; d * d <= j; ++d) {
                if (j % d == 0) {
                    idx_t m = j / d;          /* µ(j/d) for divisor d       */
                    int mu_val = mu[m];
                    if (mu_val != 0) {
                        seq_t term = pow3[d - 1];
                        if (mu_val > 0) pos += term;
                        else           neg += term;
                    }

                    idx_t e = j / d;
                    if (e != d) {
                        idx_t m2 = j / e;      /* which equals d              */
                        int mu_val2 = mu[m2];
                        if (mu_val2 != 0) {
                            seq_t term = pow3[e - 1];
                            if (mu_val2 > 0) pos += term;
                            else           neg += term;
                        }
                    }
                }
            }
            cache[j] = cache[j - 1] + (pos - neg);
        }
        cache_limit = n;
    }
    return cache[n];
}
