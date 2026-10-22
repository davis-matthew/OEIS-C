/*
 * A363268  Squares (A000290) alternating with 1+squares (A002522).
 * https://oeis.org/A363268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = k^2, a(2k) = 1 + (k-1)^2
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if (n & 1) {                     /* odd index: a(2k-1) = k^2 */
        seq_t k = (nseq + 1) / 2;    /* k = (n+1)/2 */
        return k * k;
    } else {                          /* even index: a(2k) = 1 + (k-1)^2 */
        seq_t k = nseq / 2;          /* k = n/2 */
        seq_t k1 = k - 1;            /* k-1 */
        return k1 * k1 + 1;
    }
}
