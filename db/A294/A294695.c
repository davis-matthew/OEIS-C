/*
 * A294695  Number of permutations of [n] avoiding {1243, 2431, 3412}.
 * https://oeis.org/A294695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 61*a(n-2) + 172*a(n-3) - 296*a(n-4)
 *               + 322*a(n-5) - 221*a(n-6) + 92*a(n-7) - 21*a(n-8) + 2*a(n-9)
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73,
 *                 a(6)=240, a(7)=748, a(8)=2240.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of computed terms                */
static idx_t cache_len = 0;          /* number of terms actually computed      */
static size_t cache_cap = 0;         /* allocated capacity of cache array      */

/* first nine terms of the sequence */
static const seq_t init[9] = {
    (seq_t)1,   /* a(0) */
    (seq_t)1,   /* a(1) */
    (seq_t)2,   /* a(2) */
    (seq_t)6,   /* a(3) */
    (seq_t)21,  /* a(4) */
    (seq_t)73,  /* a(5) */
    (seq_t)240, /* a(6) */
    (seq_t)748, /* a(7) */
    (seq_t)2240 /* a(8) */
};

/* ensure that cache contains all terms up to index n */
static void ensure_cache_size(idx_t n)
{
    if (!cache) {                       /* first time: create cache           */
        cache_cap = 9;                  /* we know the first nine values      */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_len = 9;                  /* 9 terms have been computed         */
    }

    if (n + 1 <= cache_len)              /* already computed                    */
        return;

    /* grow capacity if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)12 * cache[i - 1];
        val -= (seq_t)61 * cache[i - 2];
        val += (seq_t)172 * cache[i - 3];
        val -= (seq_t)296 * cache[i - 4];
        val += (seq_t)322 * cache[i - 5];
        val -= (seq_t)221 * cache[i - 6];
        val += (seq_t)92  * cache[i - 7];
        val -= (seq_t)21  * cache[i - 8];
        val += (seq_t)2   * cache[i - 9];
        cache[i] = val;
    }
    cache_len = n + 1;                   /* all terms up to n are now valid   */
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
