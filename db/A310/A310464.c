/*
 * A310464  Coordination sequence Gal.5.139.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Coordination sequence for Gal.5.139.1 (A310464).
   a(n) = floor(26*n/5) + c(n)  for n > 0
   where c(n) depends on n mod 10:
      n mod 10 = 1  -> -1
      n mod 10 = 6,7,8 -> +1
      n mod 10 = 9  -> +2
      otherwise -> 0
   a(0) = 1
*/
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute floor(26*n/5) using integer arithmetic */
    seq_t base = ((seq_t)n * (seq_t)26) / (seq_t)5;

    /* correction according to n mod 10 */
    idx_t r = n % (idx_t)10;
    seq_t c;
    switch (r) {
        case 1:          c = (seq_t)-1; break;
        case 6:
        case 7:
        case 8:          c = (seq_t)1; break;
        case 9:          c = (seq_t)2; break;
        default:         c = (seq_t)0; break;
    }
    return base + c;
}
