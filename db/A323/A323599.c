/*
 * A323599  Dirichlet convolution of the identity function with omega.
 * https://oeis.org/A323599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} sigma(n/p) (sum over distinct prime divisors p
 *            of n)
 * Range:     u8 n <= 33, u16 n <= 3017, u32 n <= 9901, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* factorize n */
    uint64_t pr_n[64], ex_n[64];
    size_t cn = 0;
    factor_u64((uint64_t)n, pr_n, ex_n, &cn);

    seq_t result = 0;

    uint64_t pr_m[64], ex_m[64];
    size_t cm;

    for (size_t i = 0; i < cn; ++i) {
        uint64_t p = pr_n[i];
        idx_t m = n / p;

        factor_u64((uint64_t)m, pr_m, ex_m, &cm);

        seq_t sigma_m = 1;
        for (size_t j = 0; j < cm; ++j) {
            uint64_t q = pr_m[j];
            uint64_t ek = ex_m[j];
            seq_t pow_q = seq_pow((seq_t)q, ek + 1);
            seq_t term = (pow_q - 1) / ((seq_t)(q - 1));
            sigma_m *= term;
        }

        result += sigma_m;
    }

    return result;
}
