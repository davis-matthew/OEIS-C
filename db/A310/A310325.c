/*
 * A310325  Coordination sequence Gal.4.24.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Coordination sequence Gal.4.24.1 (OEIS A310325)
   For n <= 8 the values are taken from the b-file.  
   For n >= 9 the following closed formula holds:

      let k = n >> 2;  // integer division by 4
      r = n & 3;      // n mod 4

      r == 0 : a(n) = 3 * (6*k + 2)
      r == 1 : a(n) = 3 * (6*k + 1)
      r == 2 : a(n) = 3 * (8*k + 2)
      r == 3 : a(n) = 3 * (6*k + 5)
*/
seq_t a(idx_t n)
{
    if (n <= 8) {
        static const seq_t small[9] =
        { 1, 3, 9, 15, 15, 18, 27, 33, 39 };
        return small[n];
    }
    idx_t k = n >> 2;              /* n / 4   */
    switch (n & 3) {               /* n % 4   */
        case 0: /* n ≡ 0 (mod 4) */
            return (seq_t)3 * ((seq_t)6 * k + (seq_t)2);
        case 1: /* n ≡ 1 (mod 4) */
            return (seq_t)3 * ((seq_t)6 * k + (seq_t)1);
        case 2: /* n ≡ 2 (mod 4) */
            return (seq_t)3 * ((seq_t)8 * k + (seq_t)2);
        case 3: /* n ≡ 3 (mod 4) */
            return (seq_t)3 * ((seq_t)6 * k + (seq_t)5);
    }
    /* unreachable, but required for all code paths */
    return (seq_t)0;
}
