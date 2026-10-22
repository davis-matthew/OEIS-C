/*
 * A370369  a(n) = n! + Sum_{k=1..n-1} (n-k)*k! = n! + A014145(n-1); for n >=
 *          2, number of m such that any two consecutive digits of the base-n
 *          expansion of m differ by 1 after arranging the digits in
 *          decreasing order.
 * https://oeis.org/A370369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n! + sum_{k=1}^{n-1} (n-k) * k! (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* factorial of (n-1)!  (for n=1 this stays 1) */
    seq_t fac = 1;
    seq_t sum = 0;

    for (idx_t k = 1; k < n; ++k)
    {
        fac = fac * (seq_t)k;                /* fac = k!  */
        sum = sum + (seq_t)(n - k) * fac;    /* accumulate (n-k)*k!  */
    }

    /* n!  = (n-1)! * n  */
    seq_t fac_n = fac * (seq_t)n;

    return fac_n + sum;
}
