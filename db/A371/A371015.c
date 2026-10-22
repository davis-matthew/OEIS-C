/*
 * A371015  The largest divisor of n that is the sum of 2 squares.
 * https://oeis.org/A371015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over primes p|n of p^e if p ≡ 1 (mod 4) or p = 2;
 *            p^(e - e mod 2) if p ≡ 3 (mod 4). This gives the largest divisor
 *            of n that is a sum of two squares
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;            /* sequence is defined for n ≥ 1 */
    }

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factorise N into primes and their exponents. */
    factor_u64(N, primes, exps, &cnt);

    seq_t result = 1;

    /* Build the largest divisor that is a sum of two squares. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp = (p % 4 == 3) ? (e - (e & 1ULL)) : e;
        result *= pow_uint(p, exp);
    }

    return result;
}
