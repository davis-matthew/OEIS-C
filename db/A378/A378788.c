/*
 * A378788  Lower matching number of the (n,n)-stacked book graph.
 * https://oeis.org/A378788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-3)-2*a(n-4)-a(n-6)+a(n-7) with initial
 *            values a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=10, a(6)=14, a(7)=17
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL; /* indexed from 0, cache[0] unused */
static size_t  cap    = 0;     /* total allocated slots (>= required +1) */
static idx_t   cached = 0;     /* highest index that has been computed */

/* Ensure that cache contains values up to at least idx n */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation, large enough for the first few terms */
        cap    = 32;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;       /* unused */
        /* initialise the first seven values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)17;
        cached  = 7;
    }

    /* grow the buffer if necessary */
    while ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values in order */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* recurrence only applies for i >= 8 */
        if (i <= 7) {
            continue; /* already initialised */
        }
        cache[i] = cache[i-1] + 2*cache[i-3] - 2*cache[i-4] - cache[i-6] + cache[i-7];
    }
    if (n > cached) cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */
    ensure_capacity(n);
    return cache[n];
}
