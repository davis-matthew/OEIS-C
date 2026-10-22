/*
 * A321577  a(n) = F_n mod M_n, where F_n = 2^(2^n) + 1 and M_n = 2^n - 1.
 * https://oeis.org/A321577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n mod n) + 1 for n>1, a(1) = 0
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 54, u64 n <= 65, u128 n <= 132,
 *            big any n
 * Helpers:   mod_pow_2, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/mod_pow_2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                /* a(1) = 0 */

    /* e = 2^n mod n  (0 ≤ e < n) */
    uint64_t e = mod_pow_2(2, (uint64_t)n, (uint64_t)n);

    /* a(n) = 2^e + 1 */
    seq_t val = pow_uint(2, e);
    val += 1;
    return val;
}
