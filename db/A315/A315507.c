/*
 * A315507  Coordination sequence Gal.6.199.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-11) + 56 for n ≥ 13 with initial values a(0) = 1,
 *            a(1) = 6, a(2) = 11, a(3) = 17, a(4) = 21, a(5) = 26, a(6) = 30,
 *            a(7) = 35, a(8) = 39, a(9) = 45, a(10) = 50, a(11) = 56, a(12) =
 *            62
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[13] = {
        (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)17,
        (seq_t)21, (seq_t)26, (seq_t)30, (seq_t)35,
        (seq_t)39, (seq_t)45, (seq_t)50, (seq_t)56,
        (seq_t)62
    };

    if (n < 3)
        return base[n];

    /* For n ≥ 3 we use the recurrence a(n) = a(n-11) + 56.
       Break n into q blocks of 11 steps plus a remainder r (2 ≤ r ≤ 12). */
    idx_t q = (n - 2) / 11;          /* number of full 11‑step blocks after index 2 */
    idx_t r = n - 11 * q;            /* remainder index, guaranteed 2 ≤ r ≤ 12 */

    seq_t offset = (seq_t)56 * (seq_t)q;  /* 56 added q times */
    return base[r] + offset;
}
