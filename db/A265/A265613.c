/*
 * A265613  a(n) = CatalanNumber(n+1)*n*(3*n^2+5*n+2)/((4+n)*(3+n)).
 * https://oeis.org/A265613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n+1)*n*(3*n^2+5*n+2)/((n+4)*(n+3))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence:
   a(0)=0, a(1)=1,
   a(n)=a(n‑1)*((2*(n+1))*(3*n+2)*(1+2*n))/((n‑1)*(3*n‑1)*(n+4)) for n≥2
   – all calculations are done with exact integer arithmetic and
   intermediate reductions using gcd to avoid overflow. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* memoised values */
static idx_t max_cached = 0;         /* highest index stored in cache */

/* Ensure that cache holds values up to at least n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise cache[0] = 0, cache[1] = 1 */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    if (n <= max_cached)
        return;

    /* grow cache to hold indices up to n */
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* recurrence for i >= 2 */
        seq_t a = cache[i - 1];

        /* factors of the recurrence */
        seq_t num[3];
        seq_t den[3];

        num[0] = (seq_t)(2 * (idx_t)(i + 1));        /* 2*(i+1)               */
        num[1] = (seq_t)(3 * (idx_t)i + 2);          /* 3*i+2                  */
        num[2] = (seq_t)(1 + 2 * (idx_t)i);          /* 1+2*i                  */

        den[0] = (seq_t)(i - 1);                     /* i-1                    */
        den[1] = (seq_t)(3 * (idx_t)i - 1);          /* 3*i-1                  */
        den[2] = (seq_t)(i + 4);                     /* i+4                    */

        /* cancel common factors between a and each denominator */
        for (int j = 0; j < 3; ++j) {
            seq_t g = gcd(a, den[j]);
            if (g > 1) {
                a /= g;
                den[j] /= g;
            }
        }

        /* cross-cancel between numerators and denominators */
        for (int i1 = 0; i1 < 3; ++i1) {
            for (int j2 = 0; j2 < 3; ++j2) {
                seq_t g = gcd(num[i1], den[j2]);
                if (g > 1) {
                    num[i1] /= g;
                    den[j2] /= g;
                }
            }
        }

        /* multiply numerators and divide by remaining denominators
           (they should all be 1 at this point) */
        for (int k = 0; k < 3; ++k) {
            a *= num[k];
            a /= den[k];              /* exact division by construction */
        }

        cache[i] = a;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
