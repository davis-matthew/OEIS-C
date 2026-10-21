/*
 * A264450  a(n) = n*(n + 11)*(n + 22)*(n + 33)*(n + 44)/120.
 * https://oeis.org/A264450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 11)*(n + 22)*(n + 33)*(n + 44)/120
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the product in an unsigned __int128 to avoid overflow.
       n is first cast to seq_t so that for the builds where seq_t
       is smaller than idx_t the value is still correct for the
       indices that fit in seq_t. The final division by 120 is exact. */
    unsigned __int128 p = 1;
    p *= (unsigned __int128)(seq_t)n;
    p *= (unsigned __int128)(seq_t)n + 11;
    p *= (unsigned __int128)(seq_t)n + 22;
    p *= (unsigned __int128)(seq_t)n + 33;
    p *= (unsigned __int128)(seq_t)n + 44;
    p /= 120;                     /* exact division */
    return (seq_t)p;
}
