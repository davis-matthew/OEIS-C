/*
 * A314903  Coordination sequence Gal.6.339.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314903
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
   Coordination sequence Gal.6.339.2 (OEIS A314903)
   The sequence satisfies: a(n) = a(n-12) + 60   for n ≥ 13
   with the first thirteen terms
     a(0) = 1, a(1) = 5, a(2) = 9, a(3) = 14, a(4) = 19,
     a(5) = 24, a(6) = 30, a(7) = 36, a(8) = 41, a(9) = 46,
     a(10) = 51, a(11) = 55, a(12) = 60.
   Consequently, for any n≥0:
     if n<13, a(n)=base[n];
     else if n%12==0, a(n)=base[12] + 60*(n/12-1);
     else            a(n)=base[n%12] + 60*(n/12).
*/

seq_t a(idx_t n)
{
    /* pre‑computed first 13 terms, index 0..12 */
    static const seq_t base[13] = {
        1, 5, 9, 14, 19, 24, 30, 36, 41, 46, 51, 55, 60
    };

    if (n < 13)
        return base[n];

    idx_t q = n / 12;
    idx_t r = n % 12;

    if (r == 0)
        return base[12] + (seq_t)60 * (q - 1);
    else
        return base[r] + (seq_t)60 * q;
}
