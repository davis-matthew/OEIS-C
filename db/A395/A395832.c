/*
 * A395832  Number of hexadecimal strings of length n that do not contain a
 *          specific string xx (where x is a single digit).
 * https://oeis.org/A395832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(n)=15*(a(n-1)+a(n-2)) for n>=2 (OEIS A395832)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)16;
    }

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)16;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = a1 + a0;     /* a(n-1)+a(n-2) */
        a0 = a1;
        a1 = (seq_t)15 * sum;    /* a(n)=15*sum */
    }

    return a1;
}
