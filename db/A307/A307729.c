/*
 * A307729  Sum of the smallest sides of all acute integer triangles with
 *          perimeter n.
 * https://oeis.org/A307729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= a <= floor(n/3)} Sum_{b=a}^{floor((n-a)/2)}
 *            [a+b > n-a-b && a^2 + b^2 > (n-a-b)^2] * a That is, sum the
 *            smallest side a of all acute integer triangles with perimeter n
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
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
        idx_t max_b = (n - a) / 2;   /* floor((n-a)/2) ensures b <= c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c) continue;                 /* triangle inequality a+b>c */
            seq_t lhs = (seq_t)a * a + (seq_t)b * b;   /* a^2 + b^2 */
            seq_t rhs = (seq_t)c * c;                 /* c^2   */
            if (lhs > rhs) sum += (seq_t)a;           /* acute condition and add a to sum */
        }
    }
    return sum;
}
