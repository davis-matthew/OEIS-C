/*
 * A331390  Number of binary matrices with 3 distinct columns and any number
 *          of nonzero rows with n ones in every column and rows in
 *          nonincreasing lexicographic order.
 * https://oeis.org/A331390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(((n+2)/2)^4) - 3*(n+1) + 2
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The rounding of ((n+2)/2)^4 is equivalent to ((n+2)^4 + 8) / 16. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k   = (seq_t)n + 2;          /* n + 2 */
    seq_t k2  = k * k;                 /* (n + 2)^2 */
    seq_t k4  = k2 * k2;               /* (n + 2)^4 */
    seq_t r   = (k4 + 8) / (seq_t)16;  /* round((n+2)^4 / 16) */
    seq_t res = r - 3 * ((seq_t)n + 1) + 2;
    return res;
}
