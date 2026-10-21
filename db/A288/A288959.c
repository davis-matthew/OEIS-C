/*
 * A288959  a(n) = n^2*(n^2 - 1)^2/2.
 * https://oeis.org/A288959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 - 1)^2/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)       /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* Convert the index to the sequence type */
    seq_t sn = (seq_t)n;

    /* n^2 */
    seq_t n2 = sn * sn;

    /* n^2 - 1 */
    seq_t t  = n2 - (seq_t)1;

    /* (n^2 - 1)^2 */
    seq_t t2 = t * t;

    /* n^2 * (n^2 - 1)^2 */
    seq_t prod = n2 * t2;

    /* Divide by 2 */
    seq_t res = prod / (seq_t)2;

    return res;
}
