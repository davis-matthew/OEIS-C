/*
 * A356050  a(n) = 2*A135318(n+1) - A135318(n).
 * https://oeis.org/A356050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A356050.
   Formula used:
     a(0) = 1,
     a(2k)  = 2*a(2(k-1)) + (-1)^(k+1)  for k ≥ 1,
     a(2k+1)= 2*a(2k) – 2^k               for k ≥ 0.
   This yields the sequence 1,1,3,4,5,6,11,14,21,26,43,54,…   */
#include <stddef.h>
#include "../utils/pow2.h"

/* Return the a(n) value for arbitrary n.  */
seq_t a(idx_t n)
{
    /* The recurrence is easiest to evaluate iteratively. */
    if (n == 0)
        return (seq_t)1;

    /* Compute a(2k) for k = floor(n/2). */
    idx_t k = n >> 1;           /* k = ⌊n/2⌋ */
    seq_t even_val = (seq_t)1; /* a(0) */

    for (idx_t t = 1; t <= k; ++t)
    {
        /* sign = +1 if (t+1) is even, else -1 */
        if (((t + 1) & 1) == 0)
            even_val = even_val * (seq_t)2 + (seq_t)1;
        else
            even_val = even_val * (seq_t)2 - (seq_t)1;
    }

    if ((n & 1) == 0)          /* n even: a(n) = a(2k) */
        return even_val;

    /* n odd: a(2k+1) = 2*a(2k) – 2^k */
    seq_t pow = pow2(k);
    return even_val * (seq_t)2 - pow;
}
