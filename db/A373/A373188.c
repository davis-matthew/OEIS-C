/*
 * A373188  Expansion of Sum_{k>=0} x^(4^k) / (1 - x^(4^k))^2.
 * https://oeis.org/A373188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 207, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(4*n+1) = 4*n+1
   a(4*n+2) = 4*n+2
   a(4*n+3) = 4*n+3
   a(4*n+4) = 4*n+4 + a(n+1)
   (n >= 0)
   The function returns the sequence term at index n (n >= 1). */
#include <stdint.h>

static seq_t a_rec(idx_t m)
{
    /* m is the sequence index (1‑based).  If m is not a multiple of 4,
       the term is m itself.  Otherwise the defining recurrence is
       a(m) = m + a(m/4).  The division by 4 is performed on the
       index; the recursion depth is at most O(log₄ m). */
    if ((m & 3) != 0)            /* m % 4 != 0 */
        return (seq_t)m;
    return (seq_t)m + a_rec(m / 4);
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
