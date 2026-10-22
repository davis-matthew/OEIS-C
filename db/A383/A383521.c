/*
 * A383521  Integers m such that phi(m) = (2/3)*(m+1).
 * https://oeis.org/A383521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 6^(2^(n-1)) – 1 (with a(1)=5)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – ignored for n==0 */

    seq_t res   = (seq_t)5;               /* a(1) */
    seq_t power = (seq_t)6;               /* 6^(2^0) */

    for (idx_t i = 0; i < n - 1; ++i) {
        res   = seq_mul(res, power + (seq_t)1);   /* multiply by (6^(2^i)+1) */
        power = seq_mul(power, power);           /* square → 6^(2^(i+1)) */
    }

    return res;
}
