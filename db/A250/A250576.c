/*
 * A250576  Number of (n+1) X (1+1) 0..1 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 52, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive formula: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-4);
   initial terms: a(1)=10, a(2)=24, a(3)=55, a(4)=125 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* compute the n‑th term of A250576; index n>=1 */
seq_t a(idx_t n)
{
    /* undefined for n<1, return 0 for safety */
    if (n == 0) return 0;

    /* static cache (1‑based) */
    static seq_t *cache = NULL;
    static size_t cap    = 0;      /* allocated slots (>=n+1)          */
    static idx_t  up_to  = 0;      /* largest n already computed       */

    /* grow cache if needed, ensuring space for the first 4 terms */
    if (cap <= n) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 5) newcap = 5;            /* at least indices 0..4 */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise first four terms if not already done */
    if (up_to < 1) { cache[1] = 10u; }
    if (up_to < 2) { cache[2] = 24u; }
    if (up_to < 3) { cache[3] = 55u; }
    if (up_to < 4) { cache[4] = 125u; }
    if (up_to < 4) up_to = 4;

    /* compute missing terms using the recurrence */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 1] + cache[i - 1]; /* 3*a(i-1) */
        val -= cache[i - 2];                                   /* - a(i-2) */
        val -= cache[i - 3] + cache[i - 3];                    /* - 2*a(i-3) */
        val += cache[i - 4];                                   /* + a(i-4) */
        cache[i] = val;
    }
    up_to = n;
    return cache[n];
}
