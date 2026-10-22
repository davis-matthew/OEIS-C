/*
 * A304267  Number of nX5 0..1 arrays with every element unequal to 0, 1, 5 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+10*a(n-3)+4*a(n-4)-2*a(n-5)-12*a(n-6)-5*a(n-7)-4*a(n-8)+4*a(n-9)
 *            for n>10 with initial values: a(1)=8, a(2)=7, a(3)=19, a(4)=50,
 *            a(5)=146, a(6)=404, a(7)=1074, a(8)=2990, a(9)=8316, a(10)=22660
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* highest computed index (1‑based) */
static size_t cache_cap  = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* unrecoverable failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize base values on first use */
static void init_cache(void)
{
    if (cache_size)
        return;
    ensure_capacity(10);
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)19;
    cache[4] = (seq_t)50;
    cache[5] = (seq_t)146;
    cache[6] = (seq_t)404;
    cache[7] = (seq_t)1074;
    cache[8] = (seq_t)2990;
    cache[9] = (seq_t)8316;
    cache[10] = (seq_t)22660;
    cache_size = 10;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    init_cache();

    if ((size_t)n <= cache_size)
        return cache[(size_t)n];

    /* Grow cache to accommodate up to n */
    ensure_capacity((size_t)n + 1);      /* +1 for 1‑based indexing */

    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        seq_t val = cache[k-1] + cache[k-2]
                 + (seq_t)10 * cache[k-3] + (seq_t)4 * cache[k-4]
                 - (seq_t)2  * cache[k-5] - (seq_t)12 * cache[k-6]
                 - (seq_t)5  * cache[k-7] - (seq_t)4 * cache[k-8]
                 + (seq_t)4  * cache[k-9];
        cache[k] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n];
}
