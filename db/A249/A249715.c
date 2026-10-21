/*
 * A249715  Positions of records in A249695.
 * https://oeis.org/A249715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249715: Positions of records in A249695
    The sequence has the following definition:
    a(1)=0,
    a(2)=4,
    a(3)=6,
    a(4)=13,
    a(5)=15,
    a(6)=27,
    a(7)=47,
    and for n≥8: a(n)=7*2^(n-5) - 1  (i.e. A086224(n-5))
*/
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* initial terms for n = 1..7 */
    static const seq_t init[] = {0, 4, 6, 13, 15, 27, 47};

    if (n <= 7)
        return init[n-1];

    /* a(n) = 7*2^(n-5) - 1 for n ≥ 8 */
    seq_t p   = pow2(n - 5);   /* p = 2^(n-5) */
    p = p * (seq_t)7;          /* multiply by 7 */
    return p - (seq_t)1;       /* subtract 1 */
}
