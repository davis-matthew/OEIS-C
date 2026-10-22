/*
 * A315735  Coordination sequence Gal.6.159.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315735
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
   Formula for A315735:
   Let q = n/12 and r = n%12.
   For n = 0, a(0) = 1.
   For n > 0:
       if r == 0   :  a(n) = 5*n + 1 + (2*q - 1)
       else        :  a(n) = 5*n + 1 + (2*q + base_f[r])
   where base_f[0..11] = {0,0,1,2,2,1,0,-1,-2,-2,-1,0}.
   This implements a linear recurrence with period 12.
*/
seq_t a(idx_t n)
{
    /* Base offset values for residues 0..11 */
    static const int8_t base_f[12] = {0,0,1,2,2,1,0,-1,-2,-2,-1,0};

    if (n == 0)          /* special case: a(0) = 1 */
        return 1;

    idx_t q = n / 12;    /* quotient */
    idx_t r = n % 12;    /* residue */

    /* Compute the correction term f */
    int offset;
    if (r == 0)
        offset = (int)(2 * q) - 1;   /* f for r == 0 */
    else
        offset = (int)(2 * q) + base_f[r];

    /* Base linear part */
    seq_t val = (seq_t)n * 5 + 1;

    /* Add or subtract the offset, keeping everything unsigned */
    if (offset >= 0)
        val += (seq_t)offset;
    else
        val -= (seq_t)(-offset);

    return val;
}
