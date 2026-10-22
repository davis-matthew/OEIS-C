/*
 * A316540  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+3*a(n-2)+8*a(n-3)-a(n-4)-12*a(n-5)-26*a(n-6)-2*a(n-7)
 *            +10*a(n-8)+15*a(n-9)-a(n-10)+6*a(n-11) for n > 12 initial values
 *            (1‑based): 1: 4 2: 12 3: 26 4: 53 5: 187 6: 484 7: 1262 8: 3570
 *            9: 9751 10: 26557 11: 72773 12: 199700
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t init[13] = {
    0ULL,  /* 0 unused to keep indices 1‑based */
    4ULL, 12ULL, 26ULL, 53ULL, 187ULL, 484ULL,
    1262ULL, 3570ULL, 9751ULL, 26557ULL,
    72773ULL, 199700ULL
};

/* static cache of computed values, 1‑based */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* size of allocated array, in elements */
static idx_t  cache_max  = 0;   /* highest index with a computed value */

/* ensure the cache can hold values up to `upto` (1‑based) */
static void ensure_cache(idx_t upto)
{
    if (upto <= cache_max) return;

    /* allocate or resize the cache */
    if (!cache) {
        /* initial allocation larger than the first 12 elements */
        cache_size =  16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;            /* abort on OOM; caller will crash */
    }
    while (cache_size <= upto) {
        idx_t new_size = cache_size * 2;
        if (new_size == 0) new_size = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;              /* abort on OOM; caller will crash */
        cache = tmp;
        cache_size = new_size;
    }

    /* fill initial values (1‑12) if not already stored */
    if (cache_max < 12) {
        idx_t i_start = cache_max + 1;
        for (idx_t i = i_start; i <= ((upto < 12) ? upto : 12); ++i) {
            cache[i] = init[i];
        }
    }

    /* compute remaining values using the recurrence */
    for (idx_t i = ((cache_max > 12) ? cache_max + 1 : 13); i <= upto; ++i) {
        seq_t pos = cache[i - 1];
        pos += 3 * cache[i - 2];
        pos += 8 * cache[i - 3];
        pos += 10 * cache[i - 8];
        pos += 15 * cache[i - 9];
        pos += 6 * cache[i - 11];

        seq_t neg = cache[i - 4];
        neg += 12 * cache[i - 5];
        neg += 26 * cache[i - 6];
        neg += 2 * cache[i - 7];
        neg += cache[i - 10];

        cache[i] = pos - neg;   /* guaranteed non‑negative */
    }

    cache_max = upto;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined before offset; arbitrary */
    ensure_cache(n);
    return cache[n];
}
