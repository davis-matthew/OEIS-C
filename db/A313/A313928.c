/*
 * A313928  Coordination sequence Gal.5.302.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5. For n ≥ 2 let d = n-2, k = d / 10, r = d %
 *            10. Then a(n) = 10 + 56·k + P[r], where P[0..9] =
 *            {0,6,12,18,24,30,36,41,46,51}
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;

    /* steps after the second term */
    idx_t d = n - 2;
    idx_t k = d / 10;   /* number of complete 10‑step cycles   */
    idx_t r = d % 10;   /* position inside the current cycle  */

    static const seq_t P[10] = {
        0, 6, 12, 18, 24, 30, 36, 41, 46, 51
    };

    return 10 + (seq_t)56 * (seq_t)k + P[r];
}
