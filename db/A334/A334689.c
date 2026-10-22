/*
 * A334689  Triangle read by rows: T(n,k) (0 <= k <= n) =
 *          k!*(Stirling2(n,k)+(k+1)*Stirling2(n,k+1))^2.
 * https://oeis.org/A334689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = k! * (S2(n,k) + (k+1) * S2(n,k+1))^2. a(n) enumerates
 *            the triangle T(n,k) row by row (row 0 first element, then row 1,
 *            …)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_R 2000          /* sufficient for n ≤ 10000 (max row ≈ 141) */

static seq_t prev[MAX_R];
static seq_t curr[MAX_R];

seq_t a(idx_t n)
{
    /* determine row r and column k from the flattened index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* factorial k! */
    seq_t fac = 1;
    for (idx_t i = 2; i <= k; ++i) {
        fac *= (seq_t)i;
    }

    /* initialize prev array for Stirling recurrence */
    for (idx_t i = 0; i < MAX_R; ++i) prev[i] = 0;
    prev[0] = 1;          /* S(0,0) = 1 */

    /* compute rows 1..r of Stirling numbers of the second kind */
    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = 0;
        for (idx_t col = 1; col <= row; ++col) {
            curr[col] = prev[col - 1] + (seq_t)col * prev[col];
        }
        for (idx_t col = 0; col <= row; ++col) {
            prev[col] = curr[col];
        }
    }

    /* retrieve required Stirling numbers */
    seq_t S_r_k  = prev[k];
    seq_t S_r_k1 = (k + 1 <= r) ? prev[k + 1] : (seq_t)0;

    /* compute the triangle entry */
    seq_t term   = S_r_k + (seq_t)(k + 1) * S_r_k1;
    seq_t result = fac * term * term;

    return result;
}
