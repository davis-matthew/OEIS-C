/*
 * A254051  Square array A by downward antidiagonals: A(n,k) = (3 +
 *          3^n*(2*floor(3*k/2) - 1))/6, n,k >= 1; read as A(1,1), A(1,2),
 *          A(2,1), A(1,3), A(2,2), A(3,1), ...
 * https://oeis.org/A254051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(n,k) read along antidiagonals of the square array
 *            A(n,k) = (3 + 3^n * (2*floor(3k/2) - 1)) / 6. Computed by
 *            mapping n to the (row,r) and (column,c), evaluating the first
 *            row via the closed form and applying the recurrence
 *            A_{row+1}=3*A_row-1
 * Range:     u8 n <= 25, u16 n <= 75, u32 n <= 251, u64 n <= 901,
 *            u128 n <= 3401, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // find the diagonal d such that d*(d+1)/2 >= n
    idx_t d   = 1;
    idx_t sum = 1;            // sum of 1..d
    while (sum < n) {
        ++d;
        sum += d;
    }

    // index within the diagonal
    idx_t prev   = sum - d;          // sum of 1..(d-1)
    idx_t offset = n - prev;        // 1 <= offset <= d
    idx_t r = offset;               // row
    idx_t c = d - offset + 1;       // column

    // compute the constant part for this column
    seq_t m   = (seq_t)(3 * c) / 2;  // floor(3c/2)
    seq_t val = 2 * m - 1;           // 2*floor(3c/2)-1

    // value of the first row for this column
    seq_t a_val = ((seq_t)3 + (seq_t)3 * val) / 6;

    // apply the recurrence to reach the desired row
    for (idx_t i = 1; i < r; ++i) {
        a_val = 3 * a_val - 1;
    }

    return a_val;
}
