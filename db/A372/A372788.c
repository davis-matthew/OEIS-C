/*
 * A372788  a(n) = tau(8*n) = A000005(8*n).
 * https://oeis.org/A372788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(8*n) (number of positive divisors of 8*n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Factor n.  The arrays are big enough for any 64‑bit factorisation. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* τ(8*n) = (exp₂(8*n)+1) * ∏_{p≠2}(exp_p(8*n)+1)
     * exp₂(8*n) = exp₂(n)+3, all other exponents unchanged. */
    seq_t result = (seq_t)1;
    seq_t exp2 = (seq_t)3;          /* start with the 2³ from 8 */

    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == (uint64_t)2) {
            exp2 += (seq_t)ex[i];
        } else {
            result *= (seq_t)(ex[i] + 1);
        }
    }

    result *= (exp2 + (seq_t)1);
    return result;
}
