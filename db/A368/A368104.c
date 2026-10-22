/*
 * A368104  The number of bi-unitary divisors of the powerful part of n.
 * https://oeis.org/A368104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e ∥ n} ( e if e is even or e = 1, e+1 otherwise )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(x, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        seq_t term = (seq_t)e;
        if ((e & 1) && e > 1)   /* e odd and >1  */
            term = (seq_t)(e + 1);
        result *= term;
    }
    return result;
}
