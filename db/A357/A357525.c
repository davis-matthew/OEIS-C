/*
 * A357525  Expansion of Product_{k>=1} (1 + mu(k)*x^k).
 * https://oeis.org/A357525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product:  ∏_{k≥1} (1 + μ(k)*x^k)
   a(n) = coefficient of x^n in that product.
   The value equals the sum over all subsets S of square‑free
   integers with Σ_{k∈S} k = n of ∏_{k∈S} μ(k). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

static seq_t *cache = NULL;   /* cached coefficients a[0..max_n]  */
static idx_t max_n   = 0;     /* highest n that has been computed */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Recompute the table if we need higher indices */
    if (cache == NULL || n > max_n) {
        idx_t size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, size);
        if (!tmp) return (seq_t)0;   /* OOM – return 0 */
        cache = tmp;

        /* zero all entries and set a[0] = 1 */
        for (idx_t i = 0; i < size; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;

        /* populate the table using the generating function */
        for (idx_t k = 1; k <= n; ++k) {
            int mu = mu_of(k);
            if (mu == 0) continue;
            seq_t mu_s = (seq_t)mu;
            for (idx_t m = n; m >= k; --m) {
                cache[m] += mu_s * cache[m - k];
            }
        }
        max_n = n;
    }
    return cache[n];
}
