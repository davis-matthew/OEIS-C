/*
 * A385456  Triangle read by rows, formed by reading Fibonomial coefficients
 *          (A010048) mod 2.
 * https://oeis.org/A385456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A010048(n) mod 2, i.e. the Fibonomial * coefficient of
 *            the (row n, column k) read from the triangular * array T(r,k)
 *            defined by * T(r,k) = binom(floor(r/3), floor(k/3)) mod 2 if r%3
 *            >= k%3, * 0 otherwise. * * The function computes the (row,
 *            column) corresponding to the * linear index n (ro...
 * Range:     u8 n <= 9436, u16 n <= 9436, u32 n <= 9436, u64 n <= 9436,
 *            u128 n <= 9436, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1) - 1) / 2);

    /* Correct any rounding errors from the double calculation.  */
    while (r * (r + 1) / 2 > n)
        r--;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    idx_t prevSum = r * (r + 1) / 2;   /* elements before row r */
    idx_t k = n - prevSum;             /* column index within the row */

    /* Check the modulus 3 condition.  */
    if ((r % 3) < (k % 3))
        return (seq_t)0;

    /* Parity of the binomial coefficient using Lucas' theorem.  */
    idx_t nr = r / 3;   /* floor(r/3)  */
    idx_t kk = k / 3;   /* floor(k/3)  */
    if ((kk & (~nr)) == 0)
        return (seq_t)1;
    return (seq_t)0;
}
