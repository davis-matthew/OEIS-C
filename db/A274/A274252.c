/*
 * A274252  Number of partitions of n^5 into at most three parts.
 * https://oeis.org/A274252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n^5 + 3)^2 / 12)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 108, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^5 using 128‑bit arithmetic to avoid overflow for
       64‑bit seq_t.  The final result is cast back to seq_t.  */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 m   = nn * nn * nn * nn * nn;   /* n^5 */
    unsigned __int128 t   = m + (unsigned __int128)3; /* n^5 + 3 */
    unsigned __int128 sq  = t * t;                    /* (n^5 + 3)^2 */
    unsigned __int128 res = (sq + (unsigned __int128)6) / (unsigned __int128)12;

    return (seq_t)res;
}
