/*
 * A269292  Number of 4Xn 0..3 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling three no more than
 *          once.
 * https://oeis.org/A269292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 96*a(n-1) - 3108*a(n-2) + 40720*a(n-3) - 265326*a(n-4) +
 *            931200*a(n-5) - 1766452*a(n-6) + 1678992*a(n-7) - 622521*a(n-8)
 *            for n>12 Initial values: a(1)=256 a(2)=29160 a(3)=1698732
 *            a(4)=87559668 a(5)=4204943820 a(6)=193446959076
 *            a(7)=8642330207532 a(8)=377958654569412
 *            a(9)=16263720330382476...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   parse_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/parse_seq.h"

/* Cache for already computed terms. */
static seq_t *cache = NULL;        /* array of a(1), a(2), ... */
static idx_t cache_len = 0;        /* number of terms stored          */
static bool init_done = false;     /* whether initial terms are set     */

seq_t a(idx_t n) {
    /* Initialise cache with the first 12 terms exactly once. */
    if (!init_done) {
        const char *first12_str[12] = {
            "256",
            "29160",
            "1698732",
            "87559668",
            "4204943820",
            "193446959076",
            "8642330207532",
            "377958654569412",
            "16263720330382476",
            "690974658284107428",
            "29056221832723692012",
            "1211537520551702429316"
        };
        cache = (seq_t *)malloc(sizeof(seq_t) * 12);
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = parse_seq(first12_str[i]);
        }
        cache_len = 12;
        init_done = true;
    }

    /* If the requested term is already cached, return it. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* extend the cache up to n */
    idx_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) {
        /* allocation failed; return 0 (undefined for this n) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* zero-based indices into cache:
           cache[k-2]  -> a(k-1)
           cache[k-3]  -> a(k-2)
           ...
           cache[k-9]  -> a(k-8)   (k>=13 ensures these exist) */
        seq_t val = cache[k - 2] * (seq_t)96;
        val -= cache[k - 3] * (seq_t)3108;
        val += cache[k - 4] * (seq_t)40720;
        val -= cache[k - 5] * (seq_t)265326;
        val += cache[k - 6] * (seq_t)931200;
        val -= cache[k - 7] * (seq_t)1766452;
        val += cache[k - 8] * (seq_t)1678992;
        val -= cache[k - 9] * (seq_t)622521;
        cache[k - 1] = val;
    }

    cache_len = new_len;
    return cache[n - 1];
}
