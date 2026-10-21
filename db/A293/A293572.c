/*
 * A293572  E.g.f.: exp(x/(1 + x + x^2 + x^3)).
 * https://oeis.org/A293572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in exp(x/(1+x+x^2+x^3)) Recurrence:
 *            for n ≥ 0 n*(n+5)*(n+4)*(n+3)*(n+2)*(n+1)*a(n) +
 *            2*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*a(n+1) +
 *            (n+5)*(n+4)*(n+3)*(8+3*n)*a(n+2) + (n+5)*(n+4)*(13+4*n)*a(n+3) +
 *            3*(n+4)*(n+5)*a(n+4) + (9+2*n)*a(n+5) + a(n+6) = 0
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initially a(0)=1, a(1)=1, a(2)=-1, a(3)=-5, a(4)=1, a(5)=161. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array of sequence values */
    static size_t cache_len = 0;     /* number of terms stored (size of cache) */
    static size_t cache_cap = 0;     /* allocated capacity of cache */

    /* initial terms */
    static const seq_t init[6] = {1, 1, -1, -5, 1, 161};

    /* Ensure cache is large enough to hold up to n */
    while (n >= cache_len) {
        /* If we have not yet filled the initial terms */
        if (cache_len < 6) {
            /* Allocate cache if first time */
            if (cache_cap == 0) {
                cache_cap = 64;             /* initial capacity */
                cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
            } else if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* Load next initial term */
            cache[cache_len] = init[cache_len];
            cache_len++;
            continue;
        }

        /* Compute next term using the recurrence.
           Let i be the index we want to fill; current start index k = i-6. */
        idx_t i = cache_len;
        idx_t k = i - 6;

        /* Compute the sum of the first six terms on the left hand side of the recurrence. */
        seq_t term = 0;
        /* a(k) term */
        term += (seq_t)k * (k+5) * (k+4) * (k+3) * (k+2) * (k+1) * cache[k];
        /* a(k+1) term */
        term += 2 * (k+1) * (k+2) * (k+3) * (k+4) * (k+5) * cache[k+1];
        /* a(k+2) term */
        term += (k+5) * (k+4) * (k+3) * (8 + 3*k) * cache[k+2];
        /* a(k+3) term */
        term += (k+5) * (k+4) * (13 + 4*k) * cache[k+3];
        /* a(k+4) term */
        term += 3 * (k+4) * (k+5) * cache[k+4];
        /* a(k+5) term */
        term += (9 + 2*k) * cache[k+5];

        /* a(i) is negative of the sum above */
        seq_t val = -term;

        /* Ensure cache capacity */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
