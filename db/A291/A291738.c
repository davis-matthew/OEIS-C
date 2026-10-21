/*
 * A291738  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - S - S^4.
 * https://oeis.org/A291738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 75, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291738 – p-INVERT of (1,0,1,0,0,0,0,…), p(S)=1‑S‑S⁴
 *
 * Recurrence (for n ≥ 13):
 *   a(n) = a(n‑1) + a(n‑3) + a(n‑4) + 4·a(n‑6) + 6·a(n‑8) + 4·a(n‑10) + a(n‑12)
 *
 * Initial values (n = 0…12):
 *   1, 1, 2, 4, 6, 13, 23, 43, 76, 138, 244, 444, 795
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms          */
static size_t cap   = 0;          /* allocated size (in elements)    */
static idx_t computed_max = (idx_t)-1; /* highest index that has been set */

/* first 13 terms of the sequence */
static const seq_t init[13] = {
    1, 1, 2, 4, 6, 13, 23, 43, 76, 138, 244, 444, 795
};

/* enlarge the cache so that indices up to 'needed' are stored */
static void grow_cache(idx_t needed)
{
    if ((size_t)(needed + 1) <= cap)
        return;

    size_t newcap = cap ? cap << 1 : 16;
    while (newcap < (size_t)(needed + 1))
        newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp && newcap > 0)
        abort();   /* allocation failure */

    cache = tmp;
    cap   = newcap;
}

/* ensure terms 0…n are available in the cache */
static void ensure_computed(idx_t n)
{
    if (computed_max != (idx_t)-1 && n <= computed_max)
        return;                    /* already computed */

    grow_cache(n);                /* make space for n */

    /* first time: store the 13 initial terms */
    if (computed_max == (idx_t)-1) {
        for (size_t i = 0; i <= 12; ++i)
            cache[i] = init[i];
        computed_max = 12;
        if (n <= 12) /* finished */
            return;
    }

    /* compute new terms using the recurrence for n ≥ 13 */
    for (idx_t i = (computed_max + 1 > 13 ? computed_max + 1 : 13);
         i <= n; ++i) {
        cache[i] =
            cache[i-1] + cache[i-3] + cache[i-4]
          + ((seq_t)4 * cache[i-6])
          + ((seq_t)6 * cache[i-8])
          + ((seq_t)4 * cache[i-10])
          + cache[i-12];
    }
    computed_max = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
