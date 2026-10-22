/*
 * A320524  Number of chiral pairs of a row of n colors using 6 or fewer
 *          colors.
 * https://oeis.org/A320524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (6^n - 6^ceil(n/2)) / 2 , for k = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Helpers:   pow_of_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_of_6.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) and floor(n/2) */
    idx_t e1 = (n + 1) / 2;          /* ceil(n/2)  */
    idx_t e2 = n - e1;               /* floor(n/2) */

    /* 6^ceil(n/2) and 6^floor(n/2) */
    seq_t pow1 = pow_of_6(e1);
    seq_t pow2 = pow_of_6(e2);

    /* (6^floor(n/2)) - 1  may be zero for n=1 */
    seq_t diff = pow2 - 1;

    seq_t result;
#if defined(__SIZEOF_INT128__)  /* available 128‑bit type */
    /* For the 64‑bit seq_t build we need 128‑bit arithmetic
       to avoid overflow.  For arbitrary‑precision seq_t,
       the cast is harmless because the product fits in
       the type. */
    __uint128_t prod = (__uint128_t)pow1 * (__uint128_t)diff;
    result = (seq_t)(prod / 2);
#else
    /* If no 128‑bit type is available, rely on seq_t having
       sufficient capacity (big‑int builds).  In the 64‑bit
       build this branch will not be used. */
    result = (pow1 * diff) / 2;
#endif

    return result;
}
