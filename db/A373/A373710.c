/*
 * A373710  Triangle read by rows: T(n,k) is the area of the square whose
 *          vertices divide the sides n of a circumscribed square into integer
 *          sections k and n - k, 0 <= k <= floor(n/2).
 * https://oeis.org/A373710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   T(n,k) = n^2 + 2*k^2 - 2*n*k; a(n) is the triangle read by rows
 * Range:     u8 n <= 71, u16 n <= 9045, u32 n <= 9045, u64 n <= 9045,
 *            u128 n <= 9045, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row (n) and column (k) corresponding to the global index n. */
    idx_t row = 0;
    idx_t cumulative = 0;

    while (1) {
        idx_t terms_in_row = (row >> 1) + 1;          /* floor(row/2)+1 */
        if (cumulative + terms_in_row > n)
            break;
        cumulative += terms_in_row;
        ++row;
    }

    idx_t k = n - cumulative;                        /* column within the row */

    /* Compute T(row,k) in seq_t arithmetic. */
    seq_t sr = (seq_t)row;
    seq_t sk = (seq_t)k;
    seq_t val = sr * sr + ((seq_t)2 * sk * sk) - ((seq_t)2 * sr * sk);
    return val;
}
