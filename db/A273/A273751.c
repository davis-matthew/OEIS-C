/*
 * A273751  Triangle of the natural numbers written by decreasing
 *          antidiagonals.
 * https://oeis.org/A273751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(r+j)^2 + 7 + (-1)^(r-j))/8 - j where r is the row
 *            such that r*(r+1)/2 >= n and j = n - (r-1)*r/2 (1-based column
 *            index)
 * Range:     u8 n <= 20, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 */

    /* Find the row r such that r*(r+1)/2 >= n (binary search) */
    idx_t low  = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;

    /* Compute the column index j (1-based) */
    seq_t prev = (seq_t)(r - 1) * r / 2;
    idx_t j = (idx_t)((seq_t)n - prev);          /* prev <= n, so difference fits */

    /* Evaluate the triangle entry */
    seq_t sum = (seq_t)r + (seq_t)j;
    seq_t sq  = sum * sum;
    seq_t num = 2 * sq + 7;
    if (((r - j) & 1ULL) == 0) {                 /* even difference -> +1 */
        num += 1;
    } else {                                     /* odd difference -> -1 */
        num -= 1;
    }
    seq_t res = num / 8;
    res -= (seq_t)j;
    return res;
}
