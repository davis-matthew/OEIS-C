/*
 * A314159  Coordination sequence Gal.6.624.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)-2*a(n-6)+2*a(n-7)-2*a(n-8)+2*a(n-9)-2*a(n-10)+2*a(n-11)-a(n-12),
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=16, a(4)=22, a(5)=28, a(6)=33,
 *            a(7)=38, a(8)=44, a(9)=50, a(10)=55, a(11)=61, a(12)=66
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cached_len = 0;        /* number of terms stored (>=0) */
static size_t cache_cap = 0;         /* allocated capacity of cache */

static const seq_t init_terms[13] = {
    (seq_t)1,  (seq_t)5,  (seq_t)11, (seq_t)16, (seq_t)22,
    (seq_t)28, (seq_t)33, (seq_t)38, (seq_t)44, (seq_t)50,
    (seq_t)55, (seq_t)61, (seq_t)66
};

static void ensure_cache(idx_t n)
{
    /* make sure cache stores indices 0..n */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (cached_len < 13) {
        /* initialise first 13 terms */
        for (size_t i = 0; i < 13; i++)
            cache[i] = init_terms[i];
        cached_len = 13;
    }
    /* compute remaining terms up to n */
    while (cached_len <= (size_t)n) {
        size_t i = cached_len;   /* current index to compute */
        seq_t sum = 0;
        for (int j = 1; j <= 11; j++) {
            seq_t term = (seq_t)2 * cache[i - j];
            if (j % 2 == 1)
                sum += term;   /* + */
            else
                sum -= term;   /* - */
        }
        sum -= cache[i - 12];
        cache[i] = sum;
        cached_len++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
