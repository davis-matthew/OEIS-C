/*
 * A352718  a(n) = floor(n^(3/2)) - floor(n^(1/2))^3.
 * https://oeis.org/A352718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)) - floor(sqrt(n))^3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* convert index to seq_t */
    seq_t m   = floor_sqrt(sn);    /* m = floor(sqrt(n)) */
    seq_t m3  = m * m * m;         /* m^3 */
    seq_t n3  = sn * sn * sn;      /* n^3 */
    seq_t ns3 = floor_sqrt(n3);    /* floor(sqrt(n^3)) = floor(n^(3/2)) */
    return ns3 - m3;               /* a(n) */
}
