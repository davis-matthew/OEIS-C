/*
 * A317409  Expansion of e.g.f. cos(x/(1 - x)).
 * https://oeis.org/A317409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317409 – expansion of the e.g.f. cos(x/(1‑x)).
   Recurrence (valid for n >= 0):
     a(n+4) = 2*(2*n+3)*(n+2)*(n+1)*a(n+1)
              - (6*n*n+24*n+25)*a(n+2)
              + 2*(2*n+5)*a(n+3)
              - n*(n+2)*(n+1)^2*a(n)
   Initial values:
     a(0)=1, a(1)=0, a(2)=-1, a(3)=-6. */

#include <stdlib.h>
#include <stdint.h>

/* Function that returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated capacity */
    static size_t  size   = 0;   /* number of stored terms */

    /* initialise the first four terms only once */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)-6;
        size = 4;
    }

    /* if the requested term is already present, return it */
    if ((size_t)n < size) {
        return cache[n];
    }

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n + 1) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute successive terms using the recurrence */
    for (idx_t next = size; next <= n; ++next) {
        idx_t base = next - 4;             /* the n in the recurrence */

        /* convert base index to seq_t for arithmetic */
        seq_t tn  = (seq_t)base;
        seq_t tn1 = tn + (seq_t)1;
        seq_t tn2 = tn + (seq_t)2;

        /* term0 = n*(n+2)*(n+1)^2*a(n)                */
        seq_t term0 = tn * tn2 * tn1 * tn1 * cache[base];
        /* term1 = 2*(2*n+3)*(n+2)*(n+1)*a(n+1)        */
        seq_t term1 = (seq_t)2 * ((tn * (seq_t)2) + (seq_t)3)
                      * tn2 * tn1 * cache[base + 1];
        /* term2 = (6*n^2 + 24*n + 25)*a(n+2)          */
        seq_t term2 = ((seq_t)6 * tn * tn
                      + (seq_t)24 * tn
                      + (seq_t)25) * cache[base + 2];
        /* term3 = 2*(2*n+5)*a(n+3)                    */
        seq_t term3 = (seq_t)2 * ((tn * (seq_t)2) + (seq_t)5)
                      * cache[base + 3];

        seq_t a_next = term1 - term2 + term3 - term0;
        cache[next] = a_next;
    }

    size = (size_t)n + 1;
    return cache[n];
}
