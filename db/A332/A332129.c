/*
 * A332129  a(n) = 2*(10^(2n+1)-1)/9 + 7*10^n.
 * https://oeis.org/A332129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2n+1)-1)/9 + 7*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^n and 10^(2n+1) */
    seq_t ten_n   = pow10(n);
    seq_t ten_2n1 = pow10(2*n + 1);

    /* compute a(n) while avoiding overflow of the intermediate
       2*(10^(2n+1)-1) for 64‑bit builds */
    seq_t part = (ten_2n1 - 1) / 9;  /* (10^(2n+1)-1) divisible by 9 */
    part *= 2;                       /* multiply by 2 */
    part += 7 * ten_n;               /* add 7·10^n */

    return part;
}
