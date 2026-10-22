/*
 * A315686  Coordination sequence Gal.6.209.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.6.209.4
   a(n) = 60 * floor(n/11),  if n % 11 == 0 (except n = 0)
   a(n) = 60 * floor(n/11) + c[n%11], otherwise
   where c[1..10] = {6,12,17,22,27,33,38,43,48,54} and c[0] = 0.
   a(0) = 1. */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    /* c[0] is 0, kept for array indexing convenience */
    static const seq_t c[11] = {0, 6, 12, 17, 22, 27, 33, 38, 43, 48, 54};
    idx_t q = n / 11;
    idx_t r = n % 11;
    seq_t result = (seq_t)60 * ((seq_t)q);
    if (r != 0) result += c[r];
    return result;
}
