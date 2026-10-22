/*
 * A300293  A sequence based on the period 6 sequence A151899.
 * https://oeis.org/A300293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-6) - a(n-7);
 * initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=3
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed values   */
static size_t len   = 0;         /* number of valid entries in cache*/
static size_t cap   = 0;         /* allocated capacity of cache     */

/* Grow the cache to hold indices up to n (inclusive) and
   initialise any new entries using the recurrence.           */
static void ensure_upto(idx_t n)
{
    if ((size_t)n < len) return;                    /* already ready      */

    size_t needed = (size_t)n + 1;
    if (needed > cap)                              /* increase capacity  */
    {
        size_t new_cap = cap? cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                         /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise any newly added entries */
    for (size_t i = len; i <= n; ++i)
    {
        if (i == 0) cache[i] = (seq_t)0;
        else if (i == 1) cache[i] = (seq_t)0;
        else if (i == 2) cache[i] = (seq_t)1;
        else if (i == 3) cache[i] = (seq_t)1;
        else if (i == 4) cache[i] = (seq_t)1;
        else if (i == 5) cache[i] = (seq_t)2;
        else if (i == 6) cache[i] = (seq_t)3;
        else
            cache[i] = cache[i-1] + cache[i-6] - cache[i-7];
    }

    len = needed;
}

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[(size_t)n];
}
