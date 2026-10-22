/*
 * A315068  Coordination sequence Gal.5.131.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n ≥ 1 a(n) = 1 + sum_{k=1}^{n} d(k), where d(k) =
 *            5,4,6,5,6,5,6,4,5 for k mod 9 = 0,1,2,3,4,5,6,7,8 respectively,
 *            except d(1) = 4 (special case). This matches the OEIS entry
 *            A315068
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute quotient and remainder of n divided by 9 */
    idx_t q = n / 9;
    idx_t r = n % 9;

    /* Increment values for residues 0..8 (mod 9) */
    static const seq_t d[9] = {5, 5, 4, 6, 5, 6, 5, 6, 4};

    seq_t sum = 0;
    for (int i = 0; i < 9; ++i) {
        /* count of numbers k in [1..n] with k % 9 == i */
        idx_t cnt = (i == 0) ? q : q + ((i <= r) ? 1 : 0);
        sum += (seq_t)cnt * d[i];
    }

    /* adjust for the special case k = 1 where d(1) = 4 instead of 5 */
    sum -= 1;

    return sum + 1;
}
