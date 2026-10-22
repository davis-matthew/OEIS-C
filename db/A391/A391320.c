/*
 * A391320  Squares of squarefree numbers that have at least 3 distinct prime
 *          factors.
 * https://oeis.org/A391320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A391320: a(n) is the square of the n‑th squarefree integer that has at
 * least three distinct prime factors.  The algorithm tests consecutive
 * integers for squarefreeness and the required prime‑factor count,
 * then stores their squares. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utilities supplied by the build */
#include "../utils/is_squarefree.h"     /* bool is_squarefree(seq_t x); */
#include "../utils/count_distinct_primes.h"  /* int count_distinct_primes(uint64_t n); */

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n < 1, return 0 as placeholder */
        return 0;

    /* static cache of the sequence values */
    static seq_t *terms = NULL;  /* array of squares */
    static size_t  cap   = 0;    /* allocated size of the array  */
    static idx_t   sz    = 0;    /* number of values stored      */
    static uint64_t last  = 1;   /* last integer examined (start 2) */

    /* initialise the cache lazily */
    if (!cap) {
        cap   = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate new terms until we have at least n of them */
    while (sz < n) {
        ++last;                       /* next candidate integer */
        /* test squarefreeness and prime factor count */
        if (is_squarefree(last) && count_distinct_primes(last) >= 3) {
            /* store its square; cast first to keep arbitrary‑precision types */
            terms[sz++] = (seq_t)last * (seq_t)last;

            /* (re)allocate if capacity reached   */
            if (sz >= cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – return 0 as safe fallback */
                    return 0;
                terms = tmp;
            }
        }
    }

    return terms[n - 1];
}
