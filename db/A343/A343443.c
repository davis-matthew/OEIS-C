/*
 * A343443  If n = Product (p_j^k_j) then a(n) = Product (k_j + 2), with a(1)
 *          = 1.
 * https://oeis.org/A343443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = Π_{p^k‖n} (k + 2), with a(1) = 1
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product of (exponent + 2). */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod = prod * (seq_t)(exps[i] + 2);
    }
    return prod;
}
