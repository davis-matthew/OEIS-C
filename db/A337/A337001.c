/*
 * A337001  a(n) = n! * Sum_{k=0..n} k^3 / k!.
 * https://oeis.org/A337001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^3 / k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=0; a(n)=n * (n^2 + a(n-1)) */

seq_t a(idx_t n)
{
    seq_t result = 0;                      /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;              /* cast index to seq_t */
        result = ii * (ii * ii + result);/* a(i) = i * (i^2 + a(i-1)) */
    }
    return result;
}
