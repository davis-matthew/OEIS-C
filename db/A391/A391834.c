/*
 * A391834  a(n) = (1/4) * Sum_{k=0..floor(n/3)} (n-2*k+2) * 2^(n-2*k) *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A391834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A391834:
a(n) = 8*a(n‑1) - 24*a(n‑2) + 40*a(n‑3)
        - 32*a(n‑4) - 32*a(n‑5) + 40*a(n‑6)
        - 32*a(n‑7) - 96*a(n‑8) + 32*a(n‑9)
        + 64*a(n‑10) - 16*a(n‑12)
with initial values
a(0)=1, a(1)=6, a(2)=24, a(3)=86, a(4)=320,
a(5)=1232, a(6)=4696, a(7)=17488, a(8)=64160,
a(9)=233872, a(10)=848960, a(11)=3067200. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* pre‑computed first 12 terms */
    static const seq_t init[12] = {
        (seq_t)1,   (seq_t)6,   (seq_t)24,   (seq_t)86,
        (seq_t)320, (seq_t)1232, (seq_t)4696, (seq_t)17488,
        (seq_t)64160, (seq_t)233872, (seq_t)848960, (seq_t)3067200
    };

    /* a static cache for arbitrary order calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < 12) return init[n];

    if (n < cache_len) return cache[n];

    /* initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (idx_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_len = 12;
    }

    /* grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        sum += (seq_t)8   * cache[i-1];
        sum -= (seq_t)24  * cache[i-2];
        sum += (seq_t)40  * cache[i-3];
        sum -= (seq_t)32  * cache[i-4];
        sum -= (seq_t)32  * cache[i-5];
        sum += (seq_t)40  * cache[i-6];
        sum -= (seq_t)32  * cache[i-7];
        sum -= (seq_t)96  * cache[i-8];
        sum += (seq_t)32  * cache[i-9];
        sum += (seq_t)64  * cache[i-10];
        sum -= (seq_t)16  * cache[i-12];
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
