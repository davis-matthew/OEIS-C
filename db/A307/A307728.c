/*
 * A307728  Sum of the perimeters of all acute integer triangles with
 *          perimeter n.
 * https://oeis.org/A307728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A070093(n) where A070093(n) counts acute integer *
 *            triangles with perimeter n
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_a = n / 3;
    for (idx_t a = 1; a <= max_a; ++a)
    {
        idx_t max_b = (n - a) / 2;          /* ensures a <= b <= c */
        for (idx_t b = a; b <= max_b; ++b)
        {
            /* Need triangle inequality a + b > c  ⇔ 2*(a + b) > n   */
            if ((a + b) * 2 <= n)
                continue;                  /* not a triangle, skip */
            idx_t c = n - a - b;

            /* Check acute condition: a^2 + b^2 > c^2 */
            seq_t aa = (seq_t)a;
            seq_t bb = (seq_t)b;
            seq_t cc = (seq_t)c;
            if (aa * aa + bb * bb > cc * cc)
                ++count;
        }
    }
    return (seq_t)n * count;
}
