/*
 * A248782  Number of integers k^5 that divide n!.
 * https://oeis.org/A248782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊(∑_{k≥1}⌊n/p^k⌋)/5⌋ + 1) count of integers k^5
 *            that divide n!
 * Range:     u8 n <= 44, u16 n <= 103, u32 n <= 244, u64 n <= 589,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed helper functions for prime generation and memory management */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, but safe fallback */

    /* obtain list of primes ≤ n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);

    seq_t result = 1;  /* product of factors */

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        idx_t exp = 0;
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            /* avoid overflow: stop if next multiplication would exceed n */
            if (power > n / p)
                break;
            power *= p;
        }
        /* exponent of p in k^5 must be ≤ exp, so a_p ≤ floor(exp/5) */
        seq_t factor = (seq_t)(exp / 5) + 1;
        result *= factor;
    }

    free(primes);
    return result;
}
