/*
 * A298375  Partial sums of A230584.
 * https://oeis.org/A298375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; for n>1, if n is even a(n) = (n³+6n²+14n)/12 if n is odd
 *            a(n) = (n³+6n²+11n+18)/12
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 for safety */
    if (n == 1) return (seq_t)2;   /* first term */

    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {            /* even n */
        return ((N * N * N) + (seq_t)6 * N * N + (seq_t)14 * N) / 12;
    } else {                      /* odd n */
        return ((N * N * N) + (seq_t)6 * N * N + (seq_t)11 * N + (seq_t)18) / 12;
    }
}
