/*
 * A357753  a(n) is the least square with n binary digits.
 * https://oeis.org/A357753
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A017912(n-1)^2, where A017912(k) = ceil(sqrt(2^k))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   floor_sqrt, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* behavior below offset is irrelevant */

    seq_t power = pow2(n - 1);   /* 2^(n-1) */
    seq_t m     = floor_sqrt(power);
    seq_t m2    = seq_mul(m, m);

    if (m2 < power)              /* ceil(sqrt) */
        m = m + 1;

    return seq_mul(m, m);        /* square of the ceil sqrt */
}
