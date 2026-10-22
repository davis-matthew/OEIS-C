/*
 * A313485  Coordination sequence Gal.6.254.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313485
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

/* Coordination sequence A313485:  Gal.6.254.6
   Formula: a(0..11) = {1,5,10,14,18,22,27,32,36,40,44,49}
            a(n) = a(n-12)+54  for n ≥ 13
   Offset: 0  (a(0) = 1)
   Note: for n = 12 the formula gives a(12) = a(0)+54-1 = 54.  */
seq_t a(idx_t n)
{
    /* Base values for n < 12 */
    static const seq_t base[12] = {
        1, 5, 10, 14, 18, 22, 27, 32, 36, 40, 44, 49
    };

    if (n < 12u)
        return base[n];

    idx_t q = n / 12;
    idx_t r = n % 12;

    /* Compute with seq_t to avoid overflow on small seq_t
       (the multiplication only overflows if the result does not
       fit in seq_t, which is allowed per the problem statement). */
    seq_t res = base[r] + (seq_t)q * (seq_t)54;
    if (r == 0)
        res -= 1;   /* adjustment for the n = 12, 24, ... cases */

    return res;
}
