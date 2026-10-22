/*
 * A310169  Coordination sequence Gal.5.5.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310169
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

/*  A310169: Coordination sequence Gal.5.5.1
    Formula implemented: a(n) = 3*n + b(n)
    where b(n) is defined by
      b(n) = init_offset[n mod 9]          if n < 9
      b(n) = base_offset[n mod 9] + 3*(p-1)            if n ≥ 9 and n mod 9 ∉ {4,5}
      b(n) = base_offset[n mod 9] + 7*(p-1)            if n ≥ 9 and n mod 9 ∈ {4,5}
    with p = floor(n / 9).  The two arrays below hold the constants:
      init_offset = {1,0,0,0,2,5,3,3,3}
      base_offset = {3,3,3,3,9,12,6,6,6}
*/

seq_t a(idx_t n)
{
    /* Determine period and residue */
    idx_t p = n / 9;
    int r = (int)(n % 9);   /* r in 0..8 */

    /* Bunch of local constants – small integers fit in seq_t safely */
    static const unsigned int init_offset[9] =
        {1, 0, 0, 0, 2, 5, 3, 3, 3};
    static const unsigned int base_offset[9] =
        {3, 3, 3, 3, 9,12, 6, 6, 6};

    seq_t offset;
    if (p == 0) {
        /* first period */
        offset = (seq_t)init_offset[r];
    } else {
        seq_t base = (seq_t)base_offset[r];
        if (r == 4 || r == 5) {
            /* special residues 4 and 5 acquire an extra 4 per period */
            offset = base + (seq_t)7 * (seq_t)(p - 1);
        } else {
            /* all other residues gain 3 per period */
            offset = base + (seq_t)3 * (seq_t)(p - 1);
        }
    }

    /* a(n) = 3*n + offset */
    seq_t result = (seq_t)3 * (seq_t)n + offset;
    return result;
}
