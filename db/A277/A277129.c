/*
 * A277129  Largest m < n such that 2^m == 2^n (mod n).
 * https://oeis.org/A277129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - ord_u(2), where u = n after removing all factors of
 *            2. ord_u(2) is the multiplicative order of 2 modulo u
 *            (ord_1(2)=1)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   order_of_2_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/order_of_2_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) /* safety: sequence defined for n>=1 */
        return (seq_t)0;
    idx_t nn = n;
    /* remove powers of 2 */
    while ((nn & 1) == 0)
        nn >>= 1;
    idx_t u = nn;                 /* odd part */
    idx_t t = order_of_2_mod(u);   /* order of 2 modulo u */
    return (seq_t)n - (seq_t)t;
}
