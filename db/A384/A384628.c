/*
 * A384628  a(n) = Sum_{k = 1..n} gcd(n, floor(n / k)).
 * https://oeis.org/A384628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=1..n} gcd(n, floor(n/k))
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n == 0) return 0;                                      /* safety */

    /* Resize cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *new_arr = (seq_t*)calloc(new_cap + 1, sizeof(seq_t));
        if (!new_arr) return 0;                               /* allocation failure */
        if (cache) {
            memcpy(new_arr + 1, cache + 1, sizeof(seq_t) * cache_cap);
            free(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Return previously computed value */
    if (cache[n] != 0) return cache[n];

    /* Compute a(n) using a grouping technique that runs in O(sqrt(n)) */
    seq_t result = 0;
    idx_t i = 1;
    seq_t sn = (seq_t)n;
    while (i <= n) {
        idx_t q = n / i;                /* floor(n / i) */
        idx_t next_i = n / q + 1;       /* first index with a different quotient */
        idx_t cnt = next_i - i;         /* how many k give this quotient */
        seq_t g = gcd(sn, (seq_t)q);    /* gcd(n, q) */
        result += g * (seq_t)cnt;
        i = next_i;
    }

    cache[n] = result;
    return result;
}
