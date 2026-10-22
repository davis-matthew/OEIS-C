/*
 * A367341  Numbers without comma-successors: these are the numbers k such
 *          that if the commas sequence A121805 is started at k instead of 1,
 *          there is no second term.
 * https://oeis.org/A367341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 100*10^floor((n-1)/8) + 9*((n-1)%8+1) - 91 (n≥1)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 64, u64 n <= 144, u128 n <= 296,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for offset 1, return 0 as safe default */

    idx_t block = (n - 1) / 8;         /* number of leading 9's */
    idx_t x     = (n - 1) % 8 + 1;     /* digit x in 1..8 */

    seq_t term = pow10(block + 2);     /* 100*10^block = 10^(block+2) */
    term -= (seq_t)100;                /* subtract 100 */
    term += (seq_t)9 * (seq_t)x;       /* add 9*x */
    term += (seq_t)9;                  /* add 9 (last digit) */

    return term;
}
