/*
 * A311481  Coordination sequence Gal.6.219.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311481
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
   Coordination sequence Gal.6.219.2 
   Recurrence: a(0)=1
   a(1)=4
   For n ≥ 2: a(n)=a(n-1)+d(n) where
       d(n)=6  if n≡6 (mod 11)
       d(n)=5  if n≡4 or 8 (mod 11)
       d(n)=4  otherwise
*/
seq_t a(idx_t n)
{
    seq_t val = 1;                 /* a(0) */
    if (n == 0) return val;

    /* a(1) = 4 */
    val += 3;                      /* difference for n = 1 */

    for (idx_t k = 2; k <= n; ++k) {
        switch (k % 11) {
            case 6:  val += 6; break;
            case 4:
            case 8: val += 5; break;
            default: val += 4; break;
        }
    }
    return val;
}
