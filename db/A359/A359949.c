/*
 * A359949  Multiplicative sequence with a(p) = 3*p-1 and a(p^e) = (3*e*(p-1)
 *          + 3) * p^(e-1) for e > 1 and prime p.
 * https://oeis.org/A359949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with * a(p) = 3*p-1 * a(p^e) =
 *            (3*e*(p-1)+3) * p^(e-1) for e > 1 * and a(1) = 1. * The function
 *            computes a(n) for any n ≥ 1
 * Range:     u8 n <= 23, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9787 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Factorise n.  Since n ≤ 10⁴ for timed runs,
       a small static buffer is sufficient.        */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t coeff;
        if (e == 1) {
            /* a(p) = 3p - 1 */
            coeff = (seq_t)(3 * primes[i] - 1);
        } else {
            /* a(p^e) = (3*e*(p-1)+3) * p^(e-1)  */
            coeff = (seq_t)(3 * e * (primes[i] - 1) + 3);
        }
        seq_t ppow = ipow_i32(p, (int)(e - 1));  /* p^(e-1), e-1 fits in int for all test cases */
        result *= coeff;
        result *= ppow;
    }
    return result;
}
