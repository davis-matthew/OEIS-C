/*
 * A290004  Wiener index of the n-Hanoi graph.
 * https://oeis.org/A290004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 395*a(n-2) + 1761*a(n-3) - 2916*a(n-4) +
 *            972*a(n-5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=3, a(2)=72, a(3)=1419, a(4)=26580, a(5)=487839 */

#include <stdlib.h>

#define MAX_CACHE 10000

static seq_t cache[MAX_CACHE + 1] = {0};
static idx_t cache_size = 0;

static void init_cache(void)
{
    if (cache_size == 0) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)72;
        cache[3] = (seq_t)1419;
        cache[4] = (seq_t)26580;
        cache[5] = (seq_t)487839;
        cache_size = 5;
    }
}

seq_t a(idx_t n)
{
    /* bounds are 1‑based; values below the offset are not defined */
    if (n == 0) return 0;          /* safe fallback for 0 index */

    init_cache();

    if (n <= cache_size) {
        return cache[n];
    }

    /* if we go beyond the pre‑allocated cache, compute on the fly */
    if (n > MAX_CACHE) {
        seq_t a1 = (seq_t)3;
        seq_t a2 = (seq_t)72;
        seq_t a3 = (seq_t)1419;
        seq_t a4 = (seq_t)26580;
        seq_t a5 = (seq_t)487839;

        for (idx_t i = 6; i <= n; ++i) {
            seq_t val = (seq_t)35 * a5
                       - (seq_t)395 * a4
                       + (seq_t)1761 * a3
                       - (seq_t)2916 * a2
                       + (seq_t)972 * a1;
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = val;
        }
        return a5;
    }

    /* compute and cache values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = (seq_t)35 * cache[i - 1]
                 - (seq_t)395 * cache[i - 2]
                 + (seq_t)1761 * cache[i - 3]
                 - (seq_t)2916 * cache[i - 4]
                 + (seq_t)972 * cache[i - 5];
    }
    cache_size = n;
    return cache[n];
}
