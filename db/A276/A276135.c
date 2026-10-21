/*
 * A276135  Ben Ames Williams's Monkey and Coconuts Problem.
 * https://oeis.org/A276135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)^(n-1)-1)/n if n is odd, * ((n-1)^n -1)/n if n is
 *            even
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(1) and a(2) are zero by definition */
    if (n <= 2) return 0U;

    seq_t base = (seq_t)(n - 1);
    idx_t exp  = (n % 2 == 0) ? n : n - 1;

    seq_t val = seq_pow(base, exp);
    val -= 1;                     /* (n-1)^exp - 1 */
    val /= (seq_t)n;              /* divide by n, exact division */

    return val;
}
