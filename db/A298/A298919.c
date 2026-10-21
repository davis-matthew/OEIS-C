/*
 * A298919  Number of nX3 0..1 arrays with every element equal to 1, 3, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-6)-a(n-7)-a(n-8) for n≥9; initial
 *            values: a(1)=0, a(2)=4, a(3)=1, a(4)=2, a(5)=3, a(6)=5, a(7)=8,
 *            a(8)=16
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of computed terms, 1‑based */
static idx_t  capacity   = 0;   /* allocated array size (≥ last_computed+1) */
static idx_t  last_computed = 0;/* highest n for which cache[n] is valid */

/* Ensure that cache[n] is computed and that the array is large enough. */
static void ensure_cache(idx_t n)
{
    if (n <= last_computed)
        return;                     /* nothing to do */

    /* enlarge the array if necessary */
    if (n + 1 > capacity) {
        size_t new_cap = n + 1;
        if (new_cap < 256)          /* avoid tiny reallocations */
            new_cap = 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                   /* allocation failure – give up */
            return;
        cache    = tmp;
        capacity = new_cap;
    }

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) cache[i] = (seq_t)0;
        else if (i == 2) cache[i] = (seq_t)4;
        else if (i == 3) cache[i] = (seq_t)1;
        else if (i == 4) cache[i] = (seq_t)2;
        else if (i == 5) cache[i] = (seq_t)3;
        else if (i == 6) cache[i] = (seq_t)5;
        else if (i == 7) cache[i] = (seq_t)8;
        else if (i == 8) cache[i] = (seq_t)16;
        else {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-6]
                              - cache[i-7] - cache[i-8];
        }
    }
    last_computed = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, but return 0 safely */

    ensure_cache(n);
    if (n <= last_computed && cache != NULL)
        return cache[n];
    return (seq_t)0;                /* allocation failed */
}
