/*
 * A251659  G.f. satisfies: A(x) = A(x^2 - x^3)/(1-x).
 * https://oeis.org/A251659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 36, i32 n <= 68, i64 n <= 122,
 *            i128 n <= 234, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251659
 * Recurrence: a(0) = 1, a(1) = 1
 * For n > 1: a(n) = 1 + Σ_{k=ceil((n+1)/3)}^{floor(n/2)}
 *                (-1)^n * C(k-1, n-2k) * a(k)
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static memoization cache */
    static seq_t *cache = NULL;   /* computed values */
    static size_t cap   = 0;      /* allocated size */
    static size_t used  = 0;      /* number of computed entries */

    /* Grow cache to hold up to index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;      /* minimal initial size */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Ensure first two terms are initialised */
    if (used < 2) {
        cache[0] = 1;
        if (cap > 1) cache[1] = 1;
        used = (cap > 1) ? 2 : 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)used; i <= n; ++i) {
        if (i <= 1) {
            cache[(size_t)i] = 1;
            continue;
        }

        idx_t k_start = (i + 3) / 3;   /* ceil((i+1)/3) */
        idx_t k_end   = i / 2;         /* floor(i/2) */

        seq_t sum = 0;
        seq_t sign = ((i & 1) == 0) ? 1 : -1;   /* (-1)^i */

        if (k_start <= k_end) {
            for (idx_t k = k_start; k <= k_end; ++k) {
                idx_t r = i - 2 * k;
                if (r > k - 1) continue;   /* C(k-1, r) is zero */

                seq_t bin = binomial((idx_t)(k - 1), r);  /* C(k-1, r) */
                seq_t term = bin * cache[(size_t)k];

                if (sign < 0)
                    sum -= term;
                else
                    sum += term;
            }
        }

        cache[(size_t)i] = 1 + sum;
    }

    used = (size_t)(n + 1);
    return cache[(size_t)n];
}
