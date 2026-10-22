/*
 * A310763  Coordination sequence Gal.4.19.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 41 + 21*⌊(n-10)/5⌋ + s[(n-10) mod 5] for n ≥ 10 * where s
 *            = {0,6,9,13,19}. For n < 10 the values are * a(0)=1, a(1)=4,
 *            a(2)=7, a(3)=11, a(4)=17, a(5)=19, a(6)=25, * a(7)=29, a(8)=33,
 *            a(9)=39
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first ten terms (indices 0 … 9) */
    static const seq_t first[10] = {
        1, 4, 7, 11, 17, 19, 25, 29, 33, 39
    };

    if (n < 10)
        return first[n];

    /* n ≥ 10 */
    seq_t m = (seq_t)(n - 10);          /* shift so that m = 0 at n = 10 */
    seq_t period = m / 5;               /* number of complete 5‑term cycles */
    seq_t rem = m % 5;                  /* position inside the current cycle */

    /* s(r) values for r = 0…4 */
    static const seq_t s[5] = {0, 6, 9, 13, 19};

    /* Ensure rem is small enough for array indexing */
    idx_t r = (idx_t)(rem);
    seq_t add = s[r];

    return 41 + 21 * period + add;
}
