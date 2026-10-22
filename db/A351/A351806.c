/*
 * A351806  Denominator of zeta({6}_n)/Pi^(6*n).
 * https://oeis.org/A351806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(6*2^(6*n)/(6*n + 3)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"

static idx_t exponent_factorial(idx_t p, idx_t m)
{
    idx_t sum = 0;
    idx_t div = p;
    while (div <= m) {
        sum += m / div;
        if (div > m / p) break;
        div *= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t m = 6 * n + 3;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(m, &primes, &prime_cnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exponent_factorial(p, m);

        if (p == 2) {
            idx_t sub = 1 + 6 * n;
            if (sub > e) e = 0;
            else e -= sub;
        } else if (p == 3) {
            if (1 > e) e = 0;
            else e -= 1;
        }

        if (e > 0) {
            seq_t mult = pow_uint(p, e);
            result *= mult;
        }
    }

    free(primes);
    return result;
}
