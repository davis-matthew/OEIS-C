/*
 * A338291  Matrix inverse of the rascal triangle (A077028), read across rows.
 * https://oeis.org/A338291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   T(n,k) = • 1 if k = n • -n if k = n-1 • n^2 - 3n + 3 if k = n-2
 *            • (-1)^(n-k) * (k^2 + k + 1) * (n-2)! / k! otherwise The
 *            sequence A338291 lists T(n,k) read across rows (row‑major order)
 * Range:     i8 n <= 28, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine row r and column c from linear index n
       Indexing: row r has r+1 elements, cumulative count up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t c = n - r * (r + 1) / 2;

    /* Compute T(r,c) according to the piecewise definition. */
    if (c == r) {
        return (seq_t)1;
    }
    if (c + 1 == r) {
        return -(seq_t)r;
    }
    if (c + 2 == r) {
        /* n^2 - 3n + 3 */
        idx_t t = r * r - 3 * r + 3;
        return (seq_t)t;
    }

    /* General case: 0 <= c <= r-2 */
    seq_t sign = ((r - c) & 1) ? (seq_t)-1 : (seq_t)1;
    seq_t factor = (seq_t)(c * c + c + 1);

    /* Compute (r-2)! / c! as product of integers from c+1 to r-2 */
    seq_t ratio = 1;
    for (idx_t i = c + 1; i <= r - 2; ++i) {
        ratio *= (seq_t)i;
    }

    return sign * factor * ratio;
}
