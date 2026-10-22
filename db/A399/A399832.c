/*
 * A399832  The number of divisors d of n such that gcud(d, n/d) is odd, where
 *          gcud is the greatest common unitary divisor.
 * https://oeis.org/A399832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers of n a(2^e) = e if e even,
 *            a(2^e) = e+1 if e odd a(p^e) = e+1 for odd prime p
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    static uint64_t *primes = NULL;
    static size_t  primes_cnt = 0;

    idx_t x = n;
    seq_t ans = (seq_t)1;
    idx_t limit = int_sqrt(n);

    /* regenerate primes list if needed */
    if (primes_cnt == 0 || primes[primes_cnt - 1] < limit) {
        /* free previous list if any */
        free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                ++exp;
                x /= p;
            }
            if (p == 2) {
                if ((exp & 1) == 0) {          /* exp even */
                    ans *= (seq_t)(exp);
                } else {                       /* exp odd */
                    ans *= (seq_t)(exp + 1);
                }
            } else {                            /* odd prime */
                ans *= (seq_t)(exp + 1);
            }
        }
    }

    /* any remaining prime factor > sqrt(n) */
    if (x > 1) {
        /* remaining exponent is 1 (odd prime) */
        ans *= (seq_t)2;
    }

    return ans;
}
