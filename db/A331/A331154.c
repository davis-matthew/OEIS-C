/*
 * A331154  Triangle read by rows: T(n,k) (n>=k>=1) = f(n,n-k+1) where f(n,k)
 *          = round((n/k)*round(n/k)).
 * https://oeis.org/A331154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = round((r/j)*round(r/j)) * where r is the unique row
 *            number satisfying r*(r+1)/2 >= n, * and j = r - k + 1 with k = n
 *            - (r-1)*r/2 (the position in that row)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r: smallest r with r*(r+1)/2 >= n */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t prev = sum - r;           /* sum of first r-1 rows   */
    idx_t k = n - prev;             /* position in row r (1‑based) */
    idx_t j = r - k + 1;            /* index used in f(r, j)    */

    /* Compute f(r, j) = round((r/j) * round(r/j)) */
    seq_t m  = (seq_t)r;
    seq_t jj = (seq_t)j;
    seq_t m1 = (m + jj / 2) / jj;   /* round(r / j)             */
    seq_t f  = (m * m1 + jj / 2) / jj; /* round(r * m1 / j)      */

    return f;
}
