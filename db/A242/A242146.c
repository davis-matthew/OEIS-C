/*
 * A242146  Number of length 2+5 0..n arrays with no consecutive six elements
 *          summing to more than 3*n.
 * https://oeis.org/A242146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A242146
   Recurrence: a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
                + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)
   Initial values (offset 1):
   a(1)=74, a(2)=1113, a(3)=7862, a(4)=36224,
   a(5)=126894, a(6)=367358, a(7)=924300, a(8)=2088459 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)0,
    (seq_t)74,  (seq_t)1113,   (seq_t)7862,
    (seq_t)36224, (seq_t)126894, (seq_t)367358,
    (seq_t)924300, (seq_t)2088459
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* allocate or grow cache array */
    size_t newcap = cache_cap ? cache_cap : 1;
    while (newcap <= (size_t)n)
        newcap <<= 1;
    seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newc) /* memory exhausted */
        exit(1);
    cache = newc;
    cache_cap = newcap;

    /* initialize required entries up to n (including initial values) */
    for (idx_t i = cache_max + 1; i <= n && i <= 8; ++i)
        cache[i] = init[i];

    /* compute further terms using the recurrence */
    idx_t start = cache_max + 1;
    if (start <= 8)
        start = 9; /* start after the pre‑computed block */
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 0;
        val += cache[i-1] * (seq_t)8;
        val += cache[i-3] * (seq_t)56;
        val += cache[i-5] * (seq_t)56;
        val += cache[i-7] * (seq_t)8;
        val -= cache[i-2] * (seq_t)28;
        val -= cache[i-4] * (seq_t)70;
        val -= cache[i-6] * (seq_t)28;
        val -= cache[i-8];
        cache[i] = val;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
