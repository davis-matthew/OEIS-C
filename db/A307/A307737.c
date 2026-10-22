/*
 * A307737  Take all the integer-sided acute triangles with perimeter n and
 *          sides a, b, and c such that a <= b <= c. a(n) is the sum of all
 *          the b's.
 * https://oeis.org/A307737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sum of the middle side b over all integer‑sided acute triangles
 *            with perimeter n. (Triangle sides a ≤ b ≤ c, a + b > c, and a² +
 *            b² > c².)
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t sum = 0;

    for (idx_t a = 1; a <= n / 3; ++a) {
        /* minimal b to satisfy a + b > n/2  */
        idx_t b_min_ineq = (n - 2 * a) / 2 + 1;
        if (b_min_ineq < a) b_min_ineq = a;      /* also enforce a ≤ b   */
        /* maximal b to keep b ≤ c = n - a - b   */
        idx_t b_max = (n - a) / 2;
        if (b_min_ineq > b_max) continue;

        for (idx_t b = b_min_ineq; b <= b_max; ++b) {
            idx_t c = n - a - b;          /* c is guaranteed ≥ b   */
            /* Check acute condition a² + b² > c²             */
            seq_t lhs = (seq_t)a * a + (seq_t)b * b;
            seq_t rhs = (seq_t)c * c;
            if (lhs > rhs) {
                sum += b;                 /* accumulate the middle side */
            }
        }
    }

    return sum;
}
