/*
 * A266175  Binary representation of the n-th iteration of the "Rule 5"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n for even n; * a(n) = floor(10*100^n/9) -
 *            101*10^(n-1) for odd n (OEIS A266175)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)                      /* even n */
        return pow10(n);

    /* odd n */
    idx_t exp = n;
    exp += n;                              /* exp = 2*n */

    seq_t t = pow10(exp);                     /* 10^(2n) = 100^n */
    t = t * (seq_t)10;                       /* 10*100^n */
    t = t / (seq_t)9;                        /* floor division by 9 */

    seq_t s = pow10(n - 1);                  /* 10^(n-1) */
    s = s * (seq_t)101;                     /* 101*10^(n-1) */

    return t - s;
}
