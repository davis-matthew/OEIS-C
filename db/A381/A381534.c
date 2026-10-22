/*
 * A381534  A084849 interleaved with positive even numbers.
 * https://oeis.org/A381534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even; * a(n) = 2*((n+1)/2)^2 - 3*((n+1)/2) + 2
 *            if n is odd
 * Range:     u8 n <= 24, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return (seq_t)n;
    } else {
        idx_t k = (n + 1) >> 1;        /* k = (n+1)/2 */
        seq_t sk = (seq_t)k;           /* cast to seq_t for arithmetic */
        return 2 * sk * sk - 3 * sk + 2;
    }
}
