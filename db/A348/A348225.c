/*
 * A348225  a(n) = Sum_{d|n} binomial(n,d)^d.
 * https://oeis.org/A348225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (binomial(n,d))^d
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   binomial, ipow, is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

/* Compute OEIS sequence A348225 */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return 0;

    /* shortcut for primes */
    if (is_prime(n))
        return (seq_t)n + 1;

    seq_t sum = 0;
    idx_t root = isqrt(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t b = binomial(n, d);
            seq_t term = ipow(b, (seq_t)d);
            sum += term;

            /* paired divisor n/d, if distinct */
            idx_t other = n / d;
            if (other != d) {
                seq_t b2 = binomial(n, other);
                seq_t term2 = ipow(b2, (seq_t)other);
                sum += term2;
            }
        }
    }
    return sum;
}
