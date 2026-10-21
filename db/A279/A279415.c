/*
 * A279415  Triangle read by rows: T(n,k), n>=k>=1, is the number of right
 *          isosceles triangles with integral coordinates that have a bounding
 *          box of size n X k.
 * https://oeis.org/A279415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for row r and column c (1‑based), with k = c, n = r, let T(r,k)
 *            = 0 if 2*k <= r; T(r,k) = 2 if r == 2*k - 1; T(r,k) = 4
 *            otherwise. The sequence a(n) is obtained by flattening the
 *            triangle row‑wise: a(1) = T(1,1), a(2) = T(2,1), a(3) = T(2,2),
 *            a(4) = T(3,1), ... This implements that definition
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 >= n */
    idx_t r  = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* Compute the column within that row (1‑based) */
    idx_t col = n - (sum - r);

    /* Special case for the single element row 1 */
    if (r == 1) {
        return (seq_t)0;
    }

    /* Apply the piecewise rule derived from the geometry */
    if (col * 2 <= r) {
        return (seq_t)0;
    }
    if (r == 2 * col - 1) {
        return (seq_t)2;
    }
    return (seq_t)4;
}
