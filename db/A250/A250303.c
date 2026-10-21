/*
 * A250303  Three-column array read by rows: T(n,k) = the coefficient of x^k
 *          in the expanded polynomial x^2 + (x+1)^2 + ... + (x+n-1)^2, for 0
 *          <= k <= 2.
 * https://oeis.org/A250303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   * a(3k) = k * a(3k+1) = k(k+1)(2k+1)/6 // square‑pyramidal
 *            numbers * a(3k+2) = k(k+1) // k‑th triangular number
 * Range:     u8 n <= 27, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) {
        /* a(3k) = k */
        return (seq_t)(n / 3);
    } else if (r == 1) {
        /* a(3k+1) = k(k+1)(2k+1)/6 */
        idx_t k = (n - 1) / 3;
        seq_t a1 = (seq_t)k;
        seq_t b1 = a1 + 1;
        seq_t c1 = 2 * a1 + 1;

        /* perform the division by 2 and 3 before multiplication to avoid overflow in small types */
        if ((a1 % 2) == 0) {
            a1 /= 2;
        } else if ((b1 % 2) == 0) {
            b1 /= 2;
        } else {
            c1 /= 2;
        }

        if ((a1 % 3) == 0) {
            a1 /= 3;
        } else if ((b1 % 3) == 0) {
            b1 /= 3;
        } else {
            c1 /= 3;
        }

        return a1 * b1 * c1;
    } else {
        /* r == 2: a(3k+2) = k(k+1) */
        idx_t k = (n - 2) / 3;
        seq_t kk = (seq_t)k;
        return kk * (kk + 1);
    }
}
