/*
 * A245350  Sum of digits of n written in fractional base 9/4.
 * https://oeis.org/A245350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of n written in fractional base 9/4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = A007953(A024652(n)) */
seq_t a(idx_t n)
{
    /* Run through the digits in the (9/4)-base expansion.
       The least‐significant digit is obtained as n mod 9.  After removing
       that digit and scaling by 4/9 the remaining number is processed.
       The loop terminates when the remaining number becomes zero.    */
    seq_t sum = 0;          /* sum of digits, accumulate in seq_t       */
    idx_t  m   = n;         /* working copy of n, stays within idx_t   */

    while (m != 0) {
        /* least‑significant digit, 0 ≤ d ≤ 8 */
        idx_t d = m % 9;
        sum += (seq_t)d;    /* add digit to the sum                 */

        /* next value: (m - d) is divisible by 9; (m-d)/9 * 4 is the next integer to process */
        m = ((m - d) / 9) * 4;
    }
    return sum;
}
