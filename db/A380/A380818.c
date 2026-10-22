/*
 * A380818  Numbers k such that the Diophantine equation d_r*x^r + ... +
 *          d_0*x^0 = 0 has an integer solution. k = (d_r .. d_0) in decimal
 *          notation, d_i are the digits of k.
 * https://oeis.org/A380818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   has_integer_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute force search for the nth term of OEIS A380818.
   For a candidate integer k, interpret its decimal digits as
   the coefficients of a polynomial d_r*x^r+…+d_0.
   The polynomial has an integer root in the interval [-9,0]
   (x=0 iff trailing digit is 0) iff k is a term.  The function
   generates terms in increasing order and returns the nth term
   (a(1) = 0).  The algorithm is fast enough for n up to at least
   10,000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_integer_root.h"

/* External linkage as requested. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;   /* a(1) = 0 */

    idx_t count = 1;          /* already counted the zero term */
    idx_t k = 1;

    for (/* start at 1 */; ; ++k) {
        if (has_integer_root(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
