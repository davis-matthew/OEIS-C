/*
 * A250901  Number of (3+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 77,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250901.
   Empirical recurrence (degree 9):
       a(n) = 14*a(n-1) - 85*a(n-2) + 294*a(n-3) - 639*a(n-4) + 906*a(n-5)
             - 839*a(n-6) + 490*a(n-7) - 164*a(n-8) + 24*a(n-9)
   Initial values:
       a(1)=413, a(2)=1450, a(3)=4388, a(4)=12387, a(5)=33821,
       a(6)=91030, a(7)=244200, a(8)=657443, a(9)=1784205. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* static cache for memoisation (1‑based index) */
    static seq_t *cache = NULL;   /* indices 0 … Size-1 */
    static idx_t  size   = 0;     /* allocated capacity */
    static idx_t  max_n  = 0;     /* largest n already computed */

    /* first call: allocate and initialise the cache with the 9 seed values */
    if (size == 0) {
        size = 10;                       /* array indices 0 … 9 */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */

        cache[0] = (seq_t)0;             /* unused sentinel */
        cache[1] = (seq_t)413;
        cache[2] = (seq_t)1450;
        cache[3] = (seq_t)4388;
        cache[4] = (seq_t)12387;
        cache[5] = (seq_t)33821;
        cache[6] = (seq_t)91030;
        cache[7] = (seq_t)244200;
        cache[8] = (seq_t)657443;
        cache[9] = (seq_t)1784205;
        max_n  = 9;
    }

    /* enlarge the cache if necessary (doubling strategy) */
    if (n >= size) {
        idx_t new_size = size;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        size  = new_size;
    }

    /* if the requested term is already computed, return it immediately */
    if (n <= max_n) return cache[n];

    /* compute all missing terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* compute positive and negative parts separately to avoid
           unsigned underflow.  mul_small handles small‑constant
           multiplication exactly for arbitrary‑precision types. */
        seq_t pos = 0;
        pos += mul_small(cache[i-1], 14);
        pos += mul_small(cache[i-3], 294);
        pos += mul_small(cache[i-5], 906);
        pos += mul_small(cache[i-7], 490);
        pos += mul_small(cache[i-9], 24);

        seq_t neg = 0;
        neg += mul_small(cache[i-2], 85);
        neg += mul_small(cache[i-4], 639);
        neg += mul_small(cache[i-6], 839);
        neg += mul_small(cache[i-8], 164);

        cache[i] = pos - neg;
    }

    max_n = n;     /* update memoised range */
    return cache[n];
}
