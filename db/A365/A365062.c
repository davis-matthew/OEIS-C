/*
 * A365062  Enumeration of | Sort_n(123,321) |.
 * https://oeis.org/A365062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=2^(n-1) for 1≤n≤3, a(n)=7·2^(n-4) for n>3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset case */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* 1 ≤ n ≤ 3 */
    if (n <= (idx_t)3)
        return pow2((idx_t)(n - (idx_t)1));

    /* n > 3 */
    return (seq_t)7 * pow2((idx_t)(n - (idx_t)4));
}
