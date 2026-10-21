/*
 * A254010  Numbers k such that 4k+1 and 4(k+1)+1 are primes.
 * https://oeis.org/A254010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the n‑th number k such that 4k+1 and 4(k+1)+1 are both prime.
 * The implementation searches for such k by testing consecutive integers
 * and uses the generic primality test is_prime(idx_t) from the utils.
 * Results are cached so successive calls are fast and order-independent.
 */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static cache of found k values */
    static uint64_t *kcache = NULL;
    static size_t ksize   = 0;   /* number of elements stored   */
    static size_t kcap    = 0;   /* allocated capacity          */

    if (n == 0) return 0; /* undefined for n<1 – harmless */

    /* extend the cache until we have at least n elements */
    while (ksize < n) {
        /* start trying from the next integer after the last stored k */
        uint64_t start = (ksize == 0) ? 1 : kcache[ksize - 1] + 1;

        for (uint64_t k = start; ; ++k) {
            idx_t p1 = 4 * k + 1;
            idx_t p2 = 4 * (k + 1) + 1;

            if (is_prime(p1) && is_prime(p2)) {
                /* grow the cache if needed */
                if (ksize == kcap) {
                    kcap = kcap ? kcap * 2 : 16;
                    kcache = (uint64_t *)realloc(kcache, kcap * sizeof(uint64_t));
                }
                kcache[ksize++] = k;
                break;               /* proceed to the next required term */
            }
        }
    }

    return (seq_t)kcache[n - 1];
}
