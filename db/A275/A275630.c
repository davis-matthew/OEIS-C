/*
 * A275630  a(n) = product of distinct primes dividing prime(n)^2 - 1.
 * https://oeis.org/A275630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct primes dividing prime(n)^2 - 1
 * Range:     u8 n <= 13, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, but safe return */

    uint32_t p = get_prime(n);            /* nth prime */
    uint64_t m = (uint64_t)p * p - 1;     /* p^2 - 1 */

    /* static cache for primes up to the needed limit */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_max = 0;

    uint64_t limit = int_sqrt((idx_t)m);  /* sqrt(m) needed for factorisation */

    if (limit > prime_max) {
        /* free old list if any */
        if (primes) free(primes);
        /* build a new list of all primes up to limit */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_max = limit;
    } else if (!primes) {                /* first call when limit <= 0 */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_max = limit;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = primes[i];
        if (q * q > m) break;
        if (m % q == 0) {
            result *= (seq_t)q;
            while (m % q == 0) m /= q;
            if (m == 1) break;
        }
    }

    if (m > 1)                     /* remaining prime factor (> sqrt) */
        result *= (seq_t)m;

    return result;
}
