/*
 * A378063  a(n) = (-2*n)^n * Euler(n, (n - 1)/(2*n)) for n >= 1, and a(0) =
 *          1. Main diagonal of A378066.
 * https://oeis.org/A378063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{j=0..n} binomial(n, j) * Euler(j) * (-n)^j where
 *            Euler(2m+1)=0, Euler(0)=1 and, Euler(2m) = - Sum_{k=0}^{2m-1}
 *            binomial(2m, k) * Euler(k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a memoized table of Euler numbers of the second kind
 * (A000364) and computes a(n) in O(n) time per call. */
#include <stdlib.h>

/* static cache for Euler numbers G_n (g_n).  Index 0 is always 1. */
static seq_t *euler_cache = NULL;
static idx_t euler_cache_size = 0;

/* Ensure euler_cache contains entries up to max_idx */
static void ensure_euler(idx_t max_idx) {
    if (max_idx < euler_cache_size) return;
    euler_cache = (seq_t *)realloc(euler_cache,
                                   (max_idx + 1) * sizeof(seq_t));
    if (!euler_cache) return; /* memory error: caller will fail */

    for (idx_t n = euler_cache_size; n <= max_idx; ++n) {
        if (n == 0) {
            euler_cache[0] = (seq_t)1;
            continue;
        }
        if (n & 1u) {                /* odd n -> Euler is 0 */
            euler_cache[n] = (seq_t)0;
            continue;
        }
        /* even n: compute via recurrence
           Euler(n) = - Sum_{k=0}^{n-1} binomial(n, k) * Euler(k)
        */
        seq_t sum = 0;
        seq_t binom = 1;            /* binomial(n,0) */
        for (idx_t k = 0; k < n; ++k) {
            sum += binom * euler_cache[k];
            if (k < n - 1) {
                /* update binom = C(n, k+1) from C(n,k) */
                seq_t num = (seq_t)(n - k);
                seq_t den = (seq_t)(k + 1);
                binom = binom * num / den;
            }
        }
        euler_cache[n] = -sum;
    }
    euler_cache_size = max_idx + 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Precompute Euler numbers up to n */
    ensure_euler(n);

    seq_t result = (seq_t)0;
    seq_t binom = (seq_t)1;       /* C(n,0) */
    seq_t pow_n = (seq_t)1;       /* (-n)^0 */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom * euler_cache[j] * pow_n;
        result += term;

        if (j < n) {
            /* Update binomial coefficient: C(n, j+1) */
            seq_t num = (seq_t)(n - j);
            seq_t den = (seq_t)(j + 1);
            binom = binom * num / den;

            /* Update power: multiply by (-n) */
            pow_n *= (seq_t)(- (long long)n);
        }
    }
    return result;
}
