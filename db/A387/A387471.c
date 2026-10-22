/*
 * A387471  a(n) = number of interior triple intersections for the equal-angle
 *          model with parameter m = 2*n-1.
 * https://oeis.org/A387471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 5 if n mod 5 != 0, else a(n) = 6*n + 7
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but safe for n=0 */
    seq_t base = (seq_t)6 * (seq_t)n;     /* 6 * n in seq_t arithmetic */
    if (n % 5 == 0) {
        return base + 7;
    } else {
        return base - 5;
    }
}
