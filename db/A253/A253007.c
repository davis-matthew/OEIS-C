/*
 * A253007  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 3 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A253007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(1)=a(2)=a(3)=a(4)=1 a(5)=124 , a(6)=3423 ,
 *            a(7)=33533 , a(8)=158877 , a(9)=490403 , a(10)=1156178 ,
 *            a(11)=2286874 , a(12)=4013538 , a(13)=6467242
 *            a(n)=4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4) for n>13 (off‑by‑one
 *            indices: the sequence is 1‑based)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 64, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for computed values ----------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements cached (>=1) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* initial values a(1)..a(13) --------------------------------------- */
static const seq_t init_vals[13] = {
    1ULL, 1ULL, 1ULL, 1ULL,
    124ULL, 3423ULL, 33533ULL,
    158877ULL, 490403ULL, 1156178ULL,
    2286874ULL, 4013538ULL, 6467242ULL
};

/* ensure that cache contains values up to index n (1‑based) --------- */
static void ensure_cache(idx_t n)
{
    /* nothing to do if we already have them */
    if (n <= cache_len)
        return;

    /* grow capacity if necessary */
    if (cache == NULL) {
        cache_cap = (n > 13) ? n : 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) /* out of memory – abort */
            abort();
        /* copy initial block */
        for (idx_t i = 0; i < 13 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_len = 13;
        /* zero remainder of first block if any */
        for (idx_t i = 13; i < cache_cap; ++i)
            cache[i] = 0;
    } else if (n > cache_cap) {
        /* exponential growth for amortised O(1) realloc */
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            abort();
        cache = new_cache;
        /* zero new space */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {          /* i is 0‑based index */
        /* i corresponds to a(i+1) */
        if (i < 13) {
            cache[i] = init_vals[i];
        } else {
            /* recurrence uses a(i) ... a(i-3) (0‑based) */
            seq_t t1 = cache[i-1]; /* a(i)   = a(n-1)  where n=i+1 */
            seq_t t2 = cache[i-2]; /* a(i-1) = a(n-2)  */
            seq_t t3 = cache[i-3]; /* a(i-2) = a(n-3)  */
            seq_t t4 = cache[i-4]; /* a(i-3) = a(n-4)  */
            seq_t val = (seq_t)4 * t1
                      - (seq_t)6 * t2
                      + (seq_t)4 * t3
                      - t4;
            cache[i] = val;
        }
    }
    cache_len = n;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, return 0 as sentinel */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
