/*
 * A357013  Triangle read by rows. T(n, k) = ((2*n)! * k!) / (n + k)!.
 * https://oeis.org/A357013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows where T(r,
 *            k) = ((2*r)! * k!)/(r + k)! (OEIS A357013)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that start_r = r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - start;             /* column within the row, 0 <= k <= r */

    seq_t result = 1;

    /* Multiply by integers from (r + k + 1) up to 2*r  (when r + k < 2*r) */
    for (idx_t i = r + k + 1; i <= 2 * r; ++i)
        result *= (seq_t)i;

    /* Multiply by integers from 1 up to k */
    for (idx_t i = 1; i <= k; ++i)
        result *= (seq_t)i;

    return result;
}
