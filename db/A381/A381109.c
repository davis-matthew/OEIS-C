/*
 * A381109  a(n) = (21*n^2 + 9*n + 2)/2.
 * https://oeis.org/A381109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* promote index to seq_t */
    if (sn & 1) {                 /* odd n: n = 2*k + 1  */
        seq_t k = sn >> 1;        /* k = (n-1)/2          */
        return 42*k*k + 51*k + 16;
    } else {                      /* even n: n = 2*k      */
        seq_t k = sn >> 1;        /* k = n/2              */
        return 42*k*k + 9*k + 1;
    }
}
