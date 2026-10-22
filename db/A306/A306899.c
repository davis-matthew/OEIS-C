/*
 * A306899  a(n) = Sum_{d|n} (2^d - (-1)^d)*phi(3*n/d).
 * https://oeis.org/A306899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2^d - (-1)^d) * phi(3*n/d)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   calc_phi, factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, so this never occurs */

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* generate all divisors of n */
    seq_t divisors[4096];
    size_t dlen = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t oldlen = dlen;
        for (size_t j = 0; j < oldlen; ++j) {
            seq_t base = divisors[j];
            seq_t val = base;
            for (uint64_t k = 1; k <= e; ++k) {
                val *= p;
                divisors[dlen++] = val;
            }
        }
    }

    seq_t result = 0;
    for (size_t i = 0; i < dlen; ++i) {
        seq_t d = divisors[i];
        seq_t pow = pow2((idx_t)d);                 /* 2^d */
        seq_t term;
        if (d & 1)                                 /* d odd: 2^d + 1 */
            term = pow + 1;
        else                                       /* d even: 2^d - 1 */
            term = pow - 1;

        seq_t m = (seq_t)( ( (seq_t)3 * (seq_t)n ) / d );
        seq_t phi_val = calc_phi(m);

        result += term * phi_val;
    }
    return result;
}
