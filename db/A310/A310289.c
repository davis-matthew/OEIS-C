/*
 * A310289  Coordination sequence Gal.4.19.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-2) + a(n-3) + 2*a(n-5) + a(n-7) - a(n-8) - a(n-10)
 *            for n > 16 Initial terms: a(0)=1, a(1)=3, a(2)=8, a(3)=13,
 *            a(4)=14, a(5)=19, a(6)=26, a(7)=29, a(8)=34, a(9)=37, a(10)=41,
 *            a(11)=48, a(12)=51, a(13)=53, a(14)=58, a(15)=65, a(16)=67
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache holds computed values a[0 .. cache_len-1] */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initial terms */
    static const seq_t init_vals[17] = {
        1ULL, 3ULL, 8ULL, 13ULL, 14ULL, 19ULL, 26ULL, 29ULL, 34ULL, 37ULL,
        41ULL, 48ULL, 51ULL, 53ULL, 58ULL, 65ULL, 67ULL
    };

    /* ensure cache capacity */
    if (cache == NULL) {
        cache_cap = 32; /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        /* load initial terms */
        memcpy(cache, init_vals, sizeof(init_vals));
        cache_len = 17;
    }
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if ((size_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i <= 16) {
                val = init_vals[i];
            } else {
                /* compute using the recurrence:
                   a(i) = -a(i-2) + a(i-3) + 2*a(i-5) + a(i-7) - a(i-8) - a(i-10) */
                seq_t pos = cache[i-3] + cache[i-7];
                /* 2*a(i-5) added separately */
                pos = pos + (cache[i-5] << 1);
                seq_t sub = cache[i-2] + cache[i-8] + cache[i-10];
                val = pos - sub;   /* safe: recurrence yields non‑negative result */
            }
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
