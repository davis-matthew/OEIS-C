/*
 * A308819  Product of prime powers <= n.
 * https://oeis.org/A308819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p prime, k>=1, p^k <= n} p^k (the product of all
 *            prime powers not exceeding n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    size_t cnt = 0;
    uint64_t *primes = NULL;

    /* generate all primes <= n */
    build_primes_upto(n64, &primes, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t power = p;

        /* multiply by each power p^k while it does not exceed n */
        while (power <= n64) {
            result = result * (seq_t)power;

            /* stop if next multiplication would overflow or exceed n */
            if (power > n64 / p)
                break;
            power *= p;
        }
    }

    free(primes);
    return result;
}
