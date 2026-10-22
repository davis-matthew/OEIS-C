/*
 * A383942  a(n) = (8*10^(2n) - 10^(n+1) + 2) / 9.
 * https://oeis.org/A383942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (8*10^(2n) - 10^(n+1) + 2) / 9
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n=0 is unused */
    idx_t twice = n + n;             /* 2*n */
    idx_t plus_one = n + 1;          /* n+1 */

    seq_t pow2n  = pow10(twice);     /* 10^(2n) */
    seq_t pown1  = pow10(plus_one);  /* 10^(n+1) */

    seq_t result = 8 * pow2n;        /* 8 * 10^(2n) */
    result -= pown1;                /* subtract 10^(n+1) */
    result += 2;                    /* add 2 */
    result /= 9;                    /* divide by 9 */

    return result;
}
