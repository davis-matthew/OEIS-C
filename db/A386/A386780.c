/*
 * A386780  a(n) = n^3*sigma_6(n).
 * https://oeis.org/A386780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * sigma_6(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 137,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma_6(n) is the sum of the 6th powers of the divisors of n. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Cached list of primes up to the largest n requested so far. */
    static uint64_t *primes   = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    prime_lim = 0;

    if (n > prime_lim) {
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_lim = n;
    }

    seq_t sigma = 1;
    idx_t tmp   = n;

    for (size_t i = 0; i < prime_cnt &&
                     (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp == 0) continue;

        /* Compute sum_{i=0..exp} p^{6i} = 1 + p^6 + p^{12} + ... */
        seq_t p6 = pow_uint(p, 6);
        seq_t sum = 1;
        seq_t pow_term = 1;
        for (int j = 0; j < exp; ++j) {
            pow_term *= p6;
            sum += pow_term;
        }
        sigma *= sum;
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1) {
        seq_t p6   = pow_uint(tmp, 6);
        seq_t sum  = 1 + p6;          /* exponent = 1 */
        sigma *= sum;
    }

    /* a(n) = n^3 * sigma_6(n). */
    seq_t n3 = pow_uint(n, 3);
    return n3 * sigma;
}
