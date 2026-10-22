/*
 * A364317  Irregular triangle T read by rows: T(n, k) gives the number of
 *          permutations of [n] = {1, 2, ..., n} with a cycle of length m =
 *          floor(n/2) + k = A138099(n, k), for 1 <= k <= n - floor(n/2) =
 *          ceiling(n/2).
 * https://oeis.org/A364317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(r,k) where r is the smallest integer with Σ_{i=1}^{r}
 *            ceil(i/2) ≥ n, k = n - Σ_{i=1}^{r-1} ceil(i/2), T(r,k) = r! /
 *            (floor(r/2)+k). This implements the definition of OEIS sequence
 *            A364317
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 49, u64 n <= 121,
 *            u128 n <= 306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; for safety return 0 for n == 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the row r and the position k within that row. */
    idx_t r      = 1;          /* current row number   */
    idx_t cum    = 0;          /* cumulative count before row r   */

    while (1) {
        idx_t row_len = (r + 1) / 2;   /* ceil(r/2) */
        if (cum + row_len >= n) {
            break;
        }
        cum += row_len;
        ++r;
    }

    idx_t k = n - cum;                 /* 1‑based position in row r      */
    idx_t m = (r / 2) + k;             /* floor(r/2)+k            */

    /* Compute r! / m without overflowing: multiply all numbers
       from 1 to r except m. The intermediate value never exceeds
       the final result, which stays within seq_t. */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        if (i == m) {
            continue;
        }
        result *= (seq_t)i;
    }

    return result;
}
