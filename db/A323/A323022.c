/*
 * A323022  Fourth omega of n. Number of distinct multiplicities in the prime
 *          signature of n.
 * https://oeis.org/A323022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct multiplicities of exponents in the
 *            prime factorization of n
 * Range:     u8 n <= 9605, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   _init_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the fourth ω of n: the number of distinct multiplicities
   of the exponents in n's prime signature.
   For n = 1 the value is 0; otherwise it is obtained by
   factorizing n, counting how often each exponent occurs,
   and then counting how many distinct such counts exist. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_init_primes.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    static bool initialized = false;
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!initialized) {
        _init_primes(&primes, &prime_cnt);
        initialized = true;
    }

    uint64_t m = n;
    uint64_t freq[65] = {0};        /* freq[e] = number of primes with exponent e   */
    size_t i;

    /* Trial division by pre‑computed primes */
    for (i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e < 65)
                ++freq[e];
        }
    }

    /* Remaining factor > 1 is prime with exponent 1 */
    if (m > 1)
        ++freq[1];

    /* Count distinct counts > 0 */
    bool seen[65] = {0};
    int result = 0;
    for (int e = 1; e < 65; ++e) {
        uint64_t cnt = freq[e];
        if (cnt > 0 && !seen[cnt]) {
            seen[cnt] = true;
            ++result;
        }
    }

    return (seq_t)result;
}
