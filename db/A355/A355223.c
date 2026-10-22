/*
 * A355223  The k-th rightmost digit of a(n) is the least of the k rightmost
 *          digits of n.
 * https://oeis.org/A355223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by scanning the decimal digits of n * from
 *            least significant to most significant, keeping a running *
 *            minimum. The k-th rightmost digit of a(n) equals the minimum of
 *            * the k rightmost digits of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case for n == 0: a(0) = 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t result = 0;          /* accumulated result */
    seq_t place  = 1;          /* current power of 10 */
    seq_t cur    = (seq_t)n;   /* working copy of n       */
    seq_t min_digit = (seq_t)9;/* running minimum of digits */

    /* Extract decimal digits from least significant to most significant. */
    while (cur != (seq_t)0) {
        seq_t d = cur % (seq_t)10;   /* current least significant digit */

        if (d < min_digit)
            min_digit = d;           /* update running minimum */

        result += min_digit * place; /* append new digit to result  */
        place  *= (seq_t)10;         /* advance to next position    */
        cur    /= (seq_t)10;         /* shift right by one digit     */
    }
    return result;
}
