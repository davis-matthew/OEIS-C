/*
 * A355221  The k-th leftmost digit of a(n) is the least of the k leftmost
 *          digits of n.
 * https://oeis.org/A355221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the number whose k‑th leftmost digit is the minimum of
 *            the k leftmost digits of n (in decimal). Leading zeros are
 *            ignored. For example, a(1402) = 1100. This function computes
 *            a(n) directly
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0)
        return 0;

    /* Find the value of the highest power of 10 <= n
       (this gives the most significant digit position). */
    idx_t temp = n;
    idx_t pw = 1;                   /* current power of 10 */
    while (temp >= 10) {
        temp /= 10;
        pw *= 10;
    }

    seq_t   res = 0;                 /* result accumulator */
    int     cur_min = 10;            /* current minimum digit (initially >9) */

    while (pw > 0) {
        /* Extract the current most significant digit. */
        idx_t digit = n / pw;
        n %= pw;                    /* remove the extracted digit */

        /* Update running minimum. */
        if (digit < (idx_t)cur_min)
            cur_min = (int)digit;

        /* Append the running minimum to the result. */
        res = res * 10 + (seq_t)cur_min;

        pw /= 10;                    /* move to next less significant digit */
    }

    return res;
}
