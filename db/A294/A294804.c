/*
 * A294804  Number of permutations of [n] avoiding {1324, 3142, 4231}.
 * https://oeis.org/A294804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 52, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294804: Number of permutations of [n] avoiding {1324,3142,4231}. */
/* Computation is made by the 5‑term linear recurrence for n > 6:   */
/* a(n) = 9·a(n‑1) – 32·a(n‑2) + 56·a(n‑3) – 48·a(n‑4) + 16·a(n‑5).   */
/* Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=242. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed values */
static idx_t  cache_len = 0;     /* number of elements already stored */

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                /* already computed up to n */

    /* grow array to size n+1 */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();         /* allocation failed */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; }
        else if (i == 1) { cache[i] = (seq_t)1; }
        else if (i == 2) { cache[i] = (seq_t)2; }
        else if (i == 3) { cache[i] = (seq_t)6; }
        else if (i == 4) { cache[i] = (seq_t)21; }
        else if (i == 5) { cache[i] = (seq_t)73; }
        else if (i == 6) { cache[i] = (seq_t)242; }
        else {
            /* compute a(i) using the recurrence with unsigned arithmetic.
               We first add the positive terms, then subtract the negative ones.
               All intermediate values are guaranteed to fit in seq_t for the
               sequence (if not, the arbitrary‑precision build will handle it). */
            seq_t t1 = cache[i-1] * (seq_t)9;   /* 9*a[i-1]   */
            seq_t t2 = cache[i-2] * (seq_t)32;  /* 32*a[i-2]  */
            seq_t t3 = cache[i-3] * (seq_t)56;  /* 56*a[i-3]  */
            seq_t t4 = cache[i-4] * (seq_t)48;  /* 48*a[i-4]  */
            seq_t t5 = cache[i-5] * (seq_t)16;  /* 16*a[i-5]  */

            seq_t sum = t1;
            sum += t3;
            sum += t5;               /* sum = 9*a[i-1] + 56*a[i-3] + 16*a[i-5] */

            seq_t sub = t2;
            sub += t4;               /* sub = 32*a[i-2] + 48*a[i-4] */

            /* 'sum' is always ≥ 'sub' for n ≥ 7, so subtraction is safe. */
            cache[i] = sum - sub;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
