/*
 * A289946  a(n) = Sum_{k=1..n} k!^6.
 * https://oeis.org/A289946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=1..n} (k!)^6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cumulative sum of k!^6 for k = 1 .. n */
    seq_t sum   = 0;          /* result */
    seq_t fac   = 1;          /* current factorial k! */
    seq_t fac6  = 1;          /* current factorial raised to the 6th power */

    if (n >= 1) {
        /* k = 1 */
        sum += fac6;          /* 1!^6 = 1 */
    }

    for (idx_t k = 2; k <= n; ++k) {
        seq_t kseq = (seq_t)k;    /* convert k to the arithmetic type */

        /* compute k^6 as (k^3)^2 to minimise multiplications */
        seq_t k_sq  = kseq * kseq;          /* k^2   */
        seq_t k_cb  = k_sq * kseq;          /* k^3   */
        seq_t k_pow6 = k_cb * k_cb;         /* k^6   */

        fac  *= kseq;       /* update factorial: k! = (k-1)! * k */
        fac6 *= k_pow6;     /* update (k!)^6: multiply by k^6 */

        sum += fac6;        /* accumulate into the running total */
    }

    return sum;
}
