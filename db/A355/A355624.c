/*
 * A355624  a(0) = 0, and for any n > 0, a(3*n) = 3*a(n), a(3*n+1) = 1-3*a(n),
 *          a(3*n+2) = 2-3*a(n).
 * https://oeis.org/A355624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(0)=0; a(3*n)=3*a(n); a(3*n+1)=1-3*a(n); a(3*n+2)=2-3*a(n)
 * Range:     i8 n <= 161, i16 n <= 6561, i32 n <= 6561, i64 n <= 6561,
 *            i128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;
    idx_t q = n / 3ULL;
    idx_t r = n - 3ULL * q;          /* r = n % 3 */
    seq_t v = a(q);
    if (r == 0ULL) return 3 * v;
    if (r == 1ULL) return 1 - 3 * v;
    /* r == 2ULL */
    return 2 - 3 * v;
}
