/*
 * A376784  Expansion of 1/sqrt((1 - x^3 - x^5)^2 - 4*x^8).
 * https://oeis.org/A376784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   reallocate_vector
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376784 (OEIS)
 *
 * G.f.: 1 / sqrt((1 - x^3 - x^5)^2 - 4*x^8)
 *
 * Let F(x) = (1 - x^3 - x^5)^2 - 4*x^8 = 1 - 2x^3 - 2x^5 + x^6 - 2x^8 + x^10.
 * Let B(x) = 1 / F(x) = Σ b_n x^n  (b_0 = 1).
 * The coefficients b_n satisfy the linear recurrence
 *
 *     b_0 = 1
 *     b_n = 2*b_{n-3} + 2*b_{n-5} - b_{n-6} + 2*b_{n-8} - b_{n-10}   (n > 0)
 *
 * Let G(x) = 1 / sqrt(F(x)) = Σ a_n x^n  (a_0 = 1).
 * Since G(x)^2 = B(x), we have
 *
 *     a_0 = 1
 *     a_n = ( b_n - Σ_{k=1}^{n-1} a_k * a_{n-k} ) / 2      (n ≥ 1)
 *
 * The implementation memoises a_n and b_n up to the largest n that has been
 * requested.  All arithmetic is performed in seq_t, and only standard
 * integer (and big‑integer) operations are used.  The code compiles both as
 * C11 and as C++17.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/reallocate_vector.h"

/* static caches (initialised on first call) */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t cache_size = 0;   /* number of elements currently stored */

seq_t a(idx_t n) {
    /* initialise caches on first use */
    if (cache_size == 0) {
#ifdef __cplusplus
        a_cache = new seq_t[1];
        b_cache = new seq_t[1];
#else
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        b_cache = (seq_t *)malloc(sizeof(seq_t));
#endif
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* extend caches if needed */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;

        a_cache = reallocate_vector(a_cache, old_size, new_size);
        b_cache = reallocate_vector(b_cache, old_size, new_size);

        for (idx_t i = old_size; i <= n; ++i) {
            /* compute b_i from its recurrence */
            seq_t bi = (seq_t)0;
            if (i >= 3) bi += (seq_t)2 * b_cache[i - 3];
            if (i >= 5) bi += (seq_t)2 * b_cache[i - 5];
            if (i >= 6) bi -= b_cache[i - 6];
            if (i >= 8) bi += (seq_t)2 * b_cache[i - 8];
            if (i >= 10) bi -= b_cache[i - 10];
            b_cache[i] = bi;

            /* compute a_i from convolution with a */
            if (i == 0) continue;
            seq_t sum = (seq_t)0;
            idx_t half = i >> 1;          /* i / 2 */
            for (idx_t k = 1; k <= half; ++k) {
                idx_t l = i - k;
                seq_t prod = a_cache[k] * a_cache[l];
                if (k == l) {
                    sum += prod;
                } else {
                    sum += prod * (seq_t)2;
                }
            }
            a_cache[i] = (b_cache[i] - sum) / ((seq_t)2);
        }
        cache_size = new_size;
    }

    return a_cache[n];
}
