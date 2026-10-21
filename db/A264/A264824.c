/*
 * A264824  Centered 17-gonal (or heptadecagonal) primes.
 * https://oeis.org/A264824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the centered 17‑gonal form (17*k^2 -
 *            17*k + 2)/2. We generate the numbers and test for primality as
 *            needed, caching results for fast repeated calls
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 15, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t len = 0;          /* number of terms stored */
    static size_t cap = 0;          /* current cache capacity */
    static idx_t k = 0;             /* last k examined */

    if (n == 0) {
        /* undefined for offset 1; return 0 as a safe value */
        return (seq_t)0;
    }

    /* grow cache until it contains at least n terms */
    while (len < n) {
        k += 1;                     /* next k */
        seq_t kk = (seq_t)k;
        /* compute centered 17-gonal number */
        seq_t v = ( ( (seq_t)17 * kk * kk
                      - (seq_t)17 * kk
                      + 2 ) / 2 );

        if (is_prime_big(v)) {
            /* ensure enough capacity */
            if (len + 1 > cap) {
                ensure_capacity(len + 1, &cap, (void**)&cache, sizeof(seq_t));
            }
            cache[len++] = v;
        }
    }

    return cache[n - 1];
}
