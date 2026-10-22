/*
 * A365672  Triangle read by rows. T(n, k) = 1 if k = 0, equals T(n, k-1) if k
 *          = n, and otherwise is (n - k + 1) * (2 * (n - k) + 1) * T(n, k -
 *          1) + T(n - 1, k).
 * https://oeis.org/A365672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle T(n,k) is defined by * T(n,0) = 1 * T(n,k) =
 *            T(n,k-1) if k == n * T(n,k) = (n-k+1)*(2*(n-k)+1)*T(n,k-1) +
 *            T(n-1,k) otherwise * The sequence a(n) is obtained by reading
 *            the triangle row‑by‑row * (neither the row nor the column are
 *            printed, only the flattened * sequence is returned). T...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* For row 0 the value is always 1. */
    if (r == 0)
        return 1;

    /* r is at most 141 for n ≤ 10000; 200 is a safe buffer. */
    seq_t prev[200], curr[200];
    prev[0] = 1; /* T(0,0) */

    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = 1; /* T(i,0) = 1 */
        for (idx_t j = 1; j <= i; ++j) {
            if (j == i) {                 /* T(i,i) = T(i,i-1) */
                curr[j] = curr[j - 1];
            } else {                       /* Recurrence */
                seq_t mult1 = (seq_t)(i - j + 1);
                seq_t mult2 = (seq_t)(2 * (i - j) + 1);
                curr[j] = mult1 * mult2 * curr[j - 1] + prev[j];
            }
        }
        /* Prepare for next row: curr becomes prev */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = curr[j];
    }
    return curr[k];
}
