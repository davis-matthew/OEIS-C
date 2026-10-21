/*
 * A244713  Positive numbers primitively represented by the binary quadratic
 *          form (1, 1, -2).
 * https://oeis.org/A244713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244713 – numbers primitively represented by the form (1,1,-2)
   Recurrence: a(1)=1, a(2)=4, a(3)=7, a(4)=10, a(5)=13, a(6)=16,
   a(7)=18, a(8)=19, a(9)=22, a(10)=25, a(11)=27, a(12)=28,
   and for n>12:  a(n) = a(n-1) + a(n-11) - a(n-12) */
#include <stdlib.h>
#include <string.h>

static const seq_t init[12] = {
    (seq_t)1, (seq_t)4, (seq_t)7, (seq_t)10, (seq_t)13,
    (seq_t)16, (seq_t)18, (seq_t)19, (seq_t)22, (seq_t)25,
    (seq_t)27, (seq_t)28
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* invalid index */

    static seq_t *cache = NULL;          /* stored values, 0‑based */
    static idx_t   cache_sz = 0;        /* number of elements stored */
    static idx_t   cache_cap = 0;        /* allocated capacity */

    /* return cached result if already computed */
    if (n <= cache_sz) return cache[n-1];

    /* grow storage to hold up to n terms */
    if (cache_cap < n)
    {
        idx_t new_cap = n;
        seq_t *new_buf = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_buf) return (seq_t)0;   /* allocation failure */
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_sz + 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 12)
        {
            val = init[i-1];
        }
        else
        {
            /* recurrence: a(i) = a(i-1) + a(i-11) - a(i-12) */
            val = cache[i-2] + cache[i-12] - cache[i-13];
        }
        cache[i-1] = val;
    }
    cache_sz = n;
    return cache[n-1];
}
