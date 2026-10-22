/*
 * A337444  Expansion of e.g.f. (1 + 2*x) * exp(x) / (sec(x) + tan(x)).
 * https://oeis.org/A337444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * (2*k+1) *
 *            A000111(n-k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

static seq_t *e_cache = NULL;    /* Euler up/down numbers A000111 */
static seq_t *a_cache = NULL;    /* wanted sequence */
static size_t cache_cap = 0;     /* number of allocated slots */

/* Compute the sequence up to index n */
seq_t a(idx_t n) {
    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;

        /* allocate new arrays */
        seq_t *new_e = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));

        /* compute Euler numbers up to n using Entringer recurrence */
        seq_t *prev = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *curr = (seq_t *)malloc(new_cap * sizeof(seq_t));

        /* base case for n = 0 */
        new_e[0] = 1;
        prev[0] = 1;   /* row for n = 0 */

        for (idx_t i = 1; i <= n; ++i) {
            curr[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                curr[k] = curr[k - 1] + prev[i - k];
            }
            new_e[i] = curr[i];

            /* swap for next iteration */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        /* free temporary row arrays */
        free(prev);
        free(curr);

        /* compute a(n) up to n */
        new_a[0] = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t bin = binomial(i, k);
                seq_t factor = (seq_t)(2 * (seq_t)k + 1);
                seq_t term = bin * factor;
                term = term * new_e[i - k];
                if ((i - k) & 1)
                    sum -= term;
                else
                    sum += term;
            }
            new_a[i] = sum;
        }

        /* free old caches */
        if (e_cache) free(e_cache);
        if (a_cache) free(a_cache);

        e_cache = new_e;
        a_cache = new_a;
        cache_cap = new_cap;
    }

    return a_cache[n];
}
