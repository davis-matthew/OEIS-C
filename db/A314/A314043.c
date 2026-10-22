/*
 * A314043  Coordination sequence Gal.6.618.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 if n % 12 == 1, 11 if n % 12 == 2, 15 if n % 12 == 3,
 *            21 if n % 12 == 4, 25 if n % 12 == 5, 31 if n % 12 == 6, 37 if n
 *            % 12 == 7, 41 if n % 12 == 8, 47 if n % 12 == 9, 51 if n % 12
 *            ==10, 57 if n % 12 ==11, and a(n+12)=a(n)+62 for all n>=1 (with
 *            a(0)=1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for indices 0..11 */
    static const seq_t base[12] = {
        1,  /* a(0) */
        5,  /* a(1) */
        11, /* a(2) */
        15, /* a(3) */
        21, /* a(4) */
        25, /* a(5) */
        31, /* a(6) */
        37, /* a(7) */
        41, /* a(8) */
        47, /* a(9) */
        51, /* a(10) */
        57  /* a(11) */
    };

    if (n == 0)
        return base[0];

    idx_t k = n / 12;          /* number of full 12-step blocks */
    idx_t r = n % 12;          /* remaining offset */

    if (r == 0)                /* n ≡ 0 (mod 12), n > 0 */
        return (seq_t)62 * k;  /* a(12k) = 62*k for k ≥ 1 */

    return base[r] + (seq_t)62 * k;
}
