/*
 * A397246  Number of partitions of an n-element labeled set into linearly
 *          ordered blocks of sizes 1, 3, or 7.
 * https://oeis.org/A397246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(0)=1;
 *            a(n)=a(n-1)+3*(n-1)*(n-2)*a(n-3)+7*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*a(n-7)
 *            for n≥0, with a(m)=0 for m<0. The recurrence follows the OEIS
 *            definition
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of entries currently stored (next free index) */
    static size_t cap  = 0;   /* allocated capacity of cache */

    /* initialise cache with a(0)=1 */
    if (len == 0) {
        cap  = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* ensure capacity for n */
    size_t idx = (size_t)n;
    if (idx >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (idx >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing values up to n */
    for (size_t i = len; i <= idx; ++i) {
        seq_t val = (seq_t)0;

        if (i >= 1) {
            val += cache[i-1];
        }
        if (i >= 3) {
            seq_t mul = (seq_t)(i-1) * (seq_t)(i-2);
            val += (seq_t)3 * mul * cache[i-3];
        }
        if (i >= 7) {
            seq_t mul = (seq_t)(i-1);
            mul *= (seq_t)(i-2);
            mul *= (seq_t)(i-3);
            mul *= (seq_t)(i-4);
            mul *= (seq_t)(i-5);
            mul *= (seq_t)(i-6);
            val += (seq_t)7 * mul * cache[i-7];
        }

        cache[i] = val;
    }

    if (len < idx + 1) len = idx + 1;
    return cache[idx];
}
