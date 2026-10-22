/*
 * A348225  a(n) = Sum_{d|n} binomial(n,d)^d.
 * https://oeis.org/A348225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (binomial(n,d))^d
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binomial, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute OEIS sequence A348225 */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return 0;

    /* shortcut for primes */
    if (is_prime(n))
        return (seq_t)n + 1;

    seq_t sum = 0;
    idx_t root = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t b = binomial(n, d);
            seq_t term = big_pow(b, (seq_t)d);
            sum += term;

            /* paired divisor n/d, if distinct */
            idx_t other = n / d;
            if (other != d) {
                seq_t b2 = binomial(n, other);
                seq_t term2 = big_pow(b2, (seq_t)other);
                sum += term2;
            }
        }
    }
    return sum;
}
