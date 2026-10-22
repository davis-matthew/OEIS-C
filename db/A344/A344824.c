/*
 * A344824  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = Sum_{j=1..n} floor(n/j) * (-k)^(j-1).
 * https://oeis.org/A344824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = A(r, k) where (r, k) are obtained by reading the * square
 *            array A(n, k) = Σ_{j=1..n} floor(n/j) * (-k)^{j-1} * by
 *            antidiagonals downwards (n≥1, k≥0)
 * Range:     i8 n <= 40, i16 n <= 84, i32 n <= 200, i64 n <= 455,
 *            i128 n <= 1208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generate function */
seq_t a(idx_t n)
{
    /* Find the antidiagonal d such that
     * S(d) = (d+1)(d+2)/2 ≥ n
     * S(d-1) = d(d+1)/2 < n
     */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < n)
        ++d;

    idx_t prevS = d * (d + 1) / 2;          /* terms before this diagonal */
    idx_t offset = n - prevS - 1;           /* 0‑based within diagonal d */
    idx_t r = offset + 1;                   /* row index (≥1)        */
    idx_t k = d - offset;                   /* column index (≥0)     */

    /* Compute A(r, k) = Σ_{j=1..r} floor(r/j) * (-k)^{j-1} */
    seq_t sum = 0;
    seq_t pow = 1;                           /* (-k)^{j-1} for j=1 */
    for (idx_t j = 1; j <= r; ++j) {
        seq_t floorDiv = (seq_t)(r / j);
        sum += floorDiv * pow;
        if (j < r)
            pow *= (seq_t)(- (long long)k);
    }
    return sum;
}
