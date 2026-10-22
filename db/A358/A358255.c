/*
 * A358255  Primitive Niven numbers ending with zero.
 * https://oeis.org/A358255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th primitive Niven number ending with zero (a Niven
 *            number divisible by 10 whose quotient by 10 is not a Niven
 *            number)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; behaviour for 0 is not required */
    idx_t count = 0;
    seq_t cand   = 10;                  /* start with the first multiple of 10 */

    while (1) {
        seq_t sum = digit_sum(cand);    /* sum of digits of cand */
        if (cand % sum == 0) {          /* cand is a Niven number */
            seq_t div10 = cand / (seq_t)10;   /* remove the trailing zero */
            seq_t sum2 = digit_sum(div10);
            if (div10 % sum2 != 0) {    /* div10 is NOT a Niven number */
                ++count;
                if (count == n) return cand;
            }
        }
        cand += 10;                     /* next number ending with zero */
    }
}
