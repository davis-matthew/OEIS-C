/*
 * A280426  Digital roots of tetranacci numbers A000078.
 * https://oeis.org/A280426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A010888(A000078(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)           /* A000078(0)=0, A000078(1)=0, A000078(2)=0 */
        return 0;

    /* Initial tetranacci values modulo 9:
       T(0)=0, T(1)=0, T(2)=0, T(3)=1 (both exact and modulo 9) */
    uint8_t t0 = 0, t1 = 0, t2 = 0, t3 = 1;

    if (n == 3)           /* digital root of 1 is 1 */
        return 1;

    /* Iterate from i = 4 to n, maintaining only the last four residues */
    for (idx_t i = 4; i <= n; ++i)
    {
        uint8_t tn = (t0 + t1 + t2 + t3) % 9;  /* T(i) mod 9 */
        t0 = t1;  t1 = t2;  t2 = t3;  t3 = tn;
    }

    /* Digital root: 0 stays 0; 1..8 stay the same; 9 for residue 0 */
    uint8_t dr = t3;
    if (dr == 0)
        dr = 9;

    return (seq_t)dr;
}
