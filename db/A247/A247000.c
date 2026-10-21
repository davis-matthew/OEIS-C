/*
 * A247000  Maximal number of palindromes in a circular binary word of length
 *          n.
 * https://oeis.org/A247000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n‑1)+a(n‑3)−a(n‑4) for n ≥ 15 Initial terms: a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=6, a(5)=7, a(6)=9, a(7)=10, a(8)=12,
 *            a(9)=13, a(10)=15, a(11)=16, a(12)=18, a(13)=19, a(14)=21
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static int base_inited = 0;

static void ensure_cache_size(idx_t need)
{
    size_t new_cap = (size_t)need + 1;      /* include index 0 */
    if (new_cap > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;                     /* unused entries start at 0 */
        cache = tmp;
        cache_cap = new_cap;
    }
}

static void init_base(void)
{
    if (base_inited) return;
    ensure_cache_size(14);
    cache[0] = 0;                          /* unused */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)6;
    cache[5] = (seq_t)7;
    cache[6] = (seq_t)9;
    cache[7] = (seq_t)10;
    cache[8] = (seq_t)12;
    cache[9] = (seq_t)13;
    cache[10] = (seq_t)15;
    cache[11] = (seq_t)16;
    cache[12] = (seq_t)18;
    cache[13] = (seq_t)19;
    cache[14] = (seq_t)21;
    base_inited = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1; undefined for n=0 */

    init_base();
    ensure_cache_size(n);

    if (n <= 14)
        return cache[(size_t)n];

    for (size_t i = 15; i <= (size_t)n; ++i)
    {
        if (cache[i] == 0)
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }
    return cache[(size_t)n];
}
