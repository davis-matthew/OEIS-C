/*
 * A367246  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 5.
 * https://oeis.org/A367246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 29*6^(n-1) - 49*5^(n-1) + 5*4^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 9, a(3) = 139, ... as listed in OEIS A367246 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1, undefined for 0 */
        return 0;

    /* n-1 is safe because n > 0 */
    idx_t n_minus1 = n - 1;

    seq_t pow6 = seq_pow((seq_t)6, n_minus1);
    seq_t pow5 = seq_pow((seq_t)5, n_minus1);
    seq_t pow4 = seq_pow((seq_t)4, n);

    seq_t sum = (seq_t)29 * pow6 + (seq_t)5 * pow4;
    seq_t res = sum - (seq_t)49 * pow5;

    return res;
}
