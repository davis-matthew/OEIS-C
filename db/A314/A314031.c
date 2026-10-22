/*
 * A314031  Coordination sequence Gal.6.340.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n+12)=a(n)+60. Initial values a(1..12) are
 *            5,11,15,20,26,30,34,40,45,49,55,60
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Periodic base for the first 12 non‑zero indices. */
    static const seq_t base[13] = {
        0,   /* dummy for index 0 */
        5, 11, 15, 20, 26, 30, 34, 40, 45, 49, 55, 60
    };

    /* Determine the r in 1..12 that corresponds to n modulo 12. */
    idx_t r = n % 12;
    if (r == 0) r = 12;

    /* Number of complete 12‑step cycles preceding n. */
    idx_t m = (n - r) / 12;

    seq_t result = base[r];
    if (m != 0) result += (seq_t)m * 60;
    return result;
}
