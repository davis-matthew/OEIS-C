/*
 * A311289  Coordination sequence Gal.5.107.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For n >= 2, let p = n - 1. Let full = p / 10,
 *            rem = p % 10. Then a(n) = 4 + full * 42 + S(rem) where S(rem) is
 *            the sum of the first rem elements of the 10-term pattern [4, 4,
 *            4, 5, 5, 4, 4, 4, 4, 4]. (S(0)=0, S(1)=4, S(2)=8, S(3)=12,
 *            S(4)=17, S(5)=22, S(6)=26, S(7)=30, S(8)=34,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Base value a(1) = 4. */
    seq_t result = 4;

    /* Compute the number of complete 10-term cycles. */
    idx_t p = n - 1;    /* number of diffs from k=2 to n, inclusive */
    idx_t full = p / 10;
    idx_t rem = p % 10;

    /* Add contribution from complete cycles: each cycle sums to 42. */
    result += (seq_t)full * 42;

    /* Partial sum of the first 'rem' terms of the patterned differences. */
    static const seq_t partial_prefix[11] = {
        0,  /* S(0) */
        4,  /* S(1) */
        8,  /* S(2) */
        12, /* S(3) */
        17, /* S(4) */
        22, /* S(5) */
        26, /* S(6) */
        30, /* S(7) */
        34, /* S(8) */
        38, /* S(9) */
        42  /* S(10) */
    };

    result += partial_prefix[rem];

    return result;
}
