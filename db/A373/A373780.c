/*
 * A373780  Positions of records in A373779.
 * https://oeis.org/A373780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is a(1)=1, a(2)=2, a(3)=23, a(4)=31, a(5)=89, a(6)=101,
 *            a(7)=404, a(8)=3026. For n ≥ 9 let m = (n + 1)/2 - 3, exp = 2*m.
 *            Then if ( (n‑9) is even ) a(n) = 10^exp + 1 else a(n) = 2·10^exp
 *            + 2 where 10^exp is computed by the utility pow10()
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static const seq_t first[] = {1, 2, 23, 31, 89, 101, 404, 3026};
    if (n <= 8) return first[n-1];

    idx_t m   = (n + 1) / 2 - 3;        /* m ≥ 2 for n ≥ 9 */
    idx_t exp = 2 * m;                  /* exponent of 10 */
    seq_t base = pow10(exp);            /* 10^exp */

    if (((n - 9) & 1) == 0)             /* first of the pair */
        return base + 1;
    else                                /* second of the pair */
        return (base << 1) + 2;        /* 2·10^exp + 2 */
}
