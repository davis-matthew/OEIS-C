/*
 * A313746  Coordination sequence Gal.5.113.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 5*n for 1 <= n < 5 a(n) = 1 + 5*n +
 *            floor((n-5)/9) for n >= 5 (i.e. each 9‑step cycle adds an extra
 *            1 after the 5‑step base)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if (n < 5) /* 1 <= n <= 4 */
        return (seq_t)5 * n;

    /* n >= 5 */
    idx_t add = (n - 5) / 9;          /* floor((n-5)/9) */
    return (seq_t)1 + (seq_t)5 * n + (seq_t)add;
}
