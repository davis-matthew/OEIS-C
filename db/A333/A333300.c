/*
 * A333300  Numbers that are of the form abab in some base (a <> b, a <> 0).
 * https://oeis.org/A333300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A333300: numbers that are of the form abab in some base (a <> b, a <> 0) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* comparison helper for qsort */
static int cmp_seq_t_func(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_sz = 0;
    static bool init = false;

    if (!init) {
        /* initial allocation */
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz = 0;

        /* generate all numbers for bases 2 .. 200 */
        for (idx_t base = 2; base <= 200; ++base) {
            seq_t pow2 = (seq_t)base * base;          /* base^2 */
            seq_t pow3 = pow2 * base;                 /* base^3 */

            for (idx_t a = 1; a < base; ++a) {
                for (idx_t b = 0; b < base; ++b) {
                    if (a == b) continue;
                    seq_t val = a * pow3 + b * pow2 + a * base + b;

                    /* append to cache */
                    if (cache_sz == cache_cap) {
                        cache_cap <<= 1;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_sz++] = val;
                }
            }
        }

        /* sort the cache */
        qsort(cache, cache_sz, sizeof(seq_t), cmp_seq_t_func);

        /* remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < cache_sz; ++i) {
            if (i == 0 || cache[i] != cache[i - 1]) {
                cache[uniq++] = cache[i];
            }
        }
        cache_sz = uniq;
        cache_cap = cache_sz;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

        init = true;
    }

    /* the sequence is 1-indexed */
    return cache[n - 1];
}
