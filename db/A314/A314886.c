/*
 * A314886  Coordination sequence Gal.5.114.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314886
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
   Coordination sequence Gal.5.114.4 (A314886).
   The sequence satisfies a periodic pattern with period 5 in the index.
   Let q = n / 5 and r = n % 5. Then

       a(n) = 23*q + offset(r,q)

   where the offset is given by

       r = 0:  offset = 0      (except for n = 0, where a(0) = 1)
       r = 1:  offset = 5 - (q & 1)      // 5 when q even, 4 when q odd
       r = 2:  offset = 9
       r = 3:  offset = 14
       r = 4:  offset = 19 - (q & 1)     // 19 when q even, 18 when q odd

   This formula reproduces all known terms and yields the correct value for
   any n ≥ 0.
*/
seq_t a(idx_t n)
{
    idx_t q = n / 5;           /* floor(n/5)      */
    idx_t r = n % 5;           /* n modulo 5     */
    seq_t base = (seq_t)q * 23;/* 23*floor(n/5)  */

    switch (r)
    {
    case 0:
        /* a(0) = 1, otherwise a(n) = base */
        if (q == 0)
            return 1;
        else
            return base;
    case 1:
        return base + (seq_t)(5 - (q & 1));
    case 2:
        return base + 9;
    case 3:
        return base + 14;
    case 4:
        return base + (seq_t)(19 - (q & 1));
    default:
        return 0;   /* unreachable */
    }
}
