/*
 * A270743  Runlength sequence of the zero-one sequence A270742.
 * https://oeis.org/A270743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 2n (n>0) /* a(3n+1) = 1 (n>=0) /* a(3n+2) = n+1 (n>=0)
 * Range:     u8 n <= 383, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t r = n % 3;
    if (r == 1)              /* a(3k+1) = 1 */
        return (seq_t)1;
    if (r == 0) {            /* a(3k) = 2k, k = n/3 */
        seq_t k = (seq_t)(n / 3);
        return k * 2;
    }                         /* r == 2 : a(3k+2) = k+1, k = (n-2)/3 */
    seq_t k = (seq_t)((n - 2) / 3);
    return k + 1;
}
