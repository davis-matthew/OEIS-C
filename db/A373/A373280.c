/*
 * A373280  Expansion of Sum_{k>=0} x^(4^k) / (1 - 4*x^(4^k)).
 * https://oeis.org/A373280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-1) if n mod 4 ≠ 0 a(n) = 4^(n-1) + a(n/4) if n mod 4
 *            = 0 (Recurrence from the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* undefined below offset; return 0 for safety */
        return 0;

    seq_t result = 0;
    while (1) {
        result += pow4(n - 1);      /* add 4^(n-1) */
        if ((n & 3) == 0) {         /* n divisible by 4? */
            n /= 4;                 /* recurse on n/4 */
            if (n == 0)             /* just in case */
                break;
        } else {
            break;                  /* termination condition */
        }
    }
    return result;
}
