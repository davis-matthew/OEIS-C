/*
 * A312683  Coordination sequence Gal.5.113.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A312683: Coordination sequence Gal.5.113.1
   Formula: a(0)=1, a(1)=4, a(2)=8;
           a(n)=a(n-1)+6  if ((n-3) mod 9 < 5)
                a(n)=a(n-1)+4  otherwise        (for n >= 3)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    seq_t val = (seq_t)8;           /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        if ( ((i - 3) % 9) < 5 )
            val += (seq_t)6;
        else
            val += (seq_t)4;
    }
    return val;
}
