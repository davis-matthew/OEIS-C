/*
 * A264132  Number of (1+1)X(n+1) arrays of permutations of 0..n*2+1 with each
 *          element having index change +-(.,.) 0,0 0,2 or 1,2.
 * https://oeis.org/A264132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 15*a(n-4) - 60*a(n-5) + 15*a(n-6) * -
 *            15*a(n-8) + 60*a(n-9) - 15*a(n-10) + a(n-12) * - 4*a(n-13) +
 *            a(n-14) for n ≥ 15 * Initial values (a(1)…a(14)): * 1, 5, 25,
 *            80, 256, 976, 3721, 13725, 50625, 189225, * 707281, 2637376,
 *            9834496, 36706880
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1].. hold values */
static idx_t cache_alloc = 0;        /* size of allocated array   */
static idx_t cache_computed = 0;     /* largest n with a cached value */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but harmless */

    /* --- initialise base values on first call --- */
    if (cache == NULL) {
        /* allocate enough space for the first 14 entries plus a guard */
        idx_t init_alloc = n < 15 ? 15 : n;
        cache = (seq_t *)malloc((init_alloc + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure unlikely */

        /* base values */
        cache[0] = (seq_t)0;       /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)80;
        cache[5] = (seq_t)256;
        cache[6] = (seq_t)976;
        cache[7] = (seq_t)3721;
        cache[8] = (seq_t)13725;
        cache[9] = (seq_t)50625;
        cache[10] = (seq_t)189225;
        cache[11] = (seq_t)707281;
        cache[12] = (seq_t)2637376;
        cache[13] = (seq_t)9834496;
        cache[14] = (seq_t)36706880;

        cache_alloc = init_alloc;
        cache_computed = 14;
    }

    /* --- grow allocation if needed --- */
    if (n > cache_alloc) {
        idx_t new_alloc = cache_alloc ? cache_alloc : 16;
        while (new_alloc < n) new_alloc <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, (new_alloc + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure unlikely */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* --- compute missing terms up to n --- */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i <= 14) continue;   /* values already known */

        seq_t val = (seq_t)0;
        val += cache[i-1] * (seq_t)4;
        val -= cache[i-2];
        val += cache[i-4] * (seq_t)15;
        val -= cache[i-5] * (seq_t)60;
        val += cache[i-6] * (seq_t)15;
        val -= cache[i-8] * (seq_t)15;
        val += cache[i-9] * (seq_t)60;
        val -= cache[i-10] * (seq_t)15;
        val += cache[i-12];
        val -= cache[i-13] * (seq_t)4;
        val += cache[i-14];
        cache[i] = val;
    }
    cache_computed = n;

    return cache[n];
}
