/*
 * A313569  Coordination sequence Gal.6.197.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n minus a small positive adjustment that depends on *
 *            the value of (n-3). For n < 3 the sequence is a(n) = 5*n with
 *            a(0)=1. * For n ≥ 3: * let m = n - 3 * let q = m / 12 (integer
 *            division) * let r = m % 12 * let pos = 2*q + (r < 7 ? 1 : 2) //
 *            positive adjustment * a(n) = 5*n - pos * * Th...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1 || n == 2)
        return (seq_t)5 * n;   /* 5*1=5, 5*2=10 */

    /* For n >= 3, compute using the formula described above. */
    idx_t m = n - 3;
    idx_t q = m / 12;
    idx_t r = m % 12;
    /* The adjustment is a small positive integer. */
    idx_t pos = 2 * q + (r < 7 ? 1 : 2);

    seq_t val = (seq_t)n * (seq_t)5;   /* 5*n */
    val -= (seq_t)pos;                 /* subtract the adjustment */

    return val;
}
