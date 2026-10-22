/*
 * A363918  a(n) = Product_{p in Factors(n)} mult(p)*n^(mult(p) - 1), where
 *          Factors(n) is the integer factorization of n and mult(p) the
 *          multiplicity of the prime factor p.
 * https://oeis.org/A363918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p in Factors(n)} mult(p)*n^(mult(p)-1)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"


seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = ex[i];
        if (e > 1)
        {
            seq_t pow_val = pow_uint(nn, (idx_t)(e - 1));
            seq_t term    = pow_val * (seq_t)e;
            result        = result * term;
        }
    }
    return result;
}
