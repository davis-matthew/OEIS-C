/*
 * A353182  Number of n-digit numbers in which more than half of the digits
 *          are the same.
 * https://oeis.org/A353182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=floor(n/2)+1..n} C(n,k)*9^(n-k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard – offset is 1 */

    const idx_t L = (n >> 1) + 1;   /* floor(n/2)+1 */

    seq_t binom = 1;   /* C(n,n) */
    seq_t pow9  = 9;   /* 9^(n-n+1) = 9 */

    seq_t sum = binom * pow9;  /* term for k = n */

    for (idx_t k = n; k > L; --k) {
        /* C(n,k-1) = C(n,k) * k / (n-k+1) */
        binom = binom * k / (n - k + 1);
        /* 9^(n-(k-1)+1) = 9^(n-k+2) = pow9 * 9 */
        pow9 = pow9 * 9;
        sum += binom * pow9;
    }
    return sum;
}
