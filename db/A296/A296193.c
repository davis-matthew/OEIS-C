/*
 * A296193  Numerators of Harary index for the n-Mycielski graph.
 * https://oeis.org/A296193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of Harary index for the n-Mycielski graph
   Formula: a(n) = ((3 + (-1)^n)*(432 - 135*2^(n + 2) + 112*3^n + 81*4^n))/1152
   (for n > 3).  For n = 1,2,3 the values are 0, 1, 15 respectively. */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)15;

    /* Compute the powers.  seq_pow is defined for all seq_t types. */
    seq_t two_pow  = seq_pow((seq_t)2, n + 2);
    seq_t three_pow = seq_pow((seq_t)3, n);
    seq_t four_pow  = seq_pow((seq_t)4, n);

    /* Compute the inner parenthesis of the formula. */
    seq_t t = (seq_t)432;
    t -= (seq_t)135 * two_pow;
    t += (seq_t)112 * three_pow;
    t += (seq_t)81  * four_pow;

    /* Apply the outer factor / denominator */
    /* Even n : factor = 4 → divide by 288.
       Odd  n : factor = 2 → divide by 576. */
    if ((n & 1ULL) == 0) {
        return t / 288;
    } else {
        return t / 576;
    }
}
