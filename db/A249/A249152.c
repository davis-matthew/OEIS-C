/*
 * A249152  Exponent of 2 in the hyperfactorials: a(n) = A007814(A002109(n)).
 * https://oeis.org/A249152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(floor(n/2)) + floor(n/2)*(floor(n/2)+1), a(0) = a(1)
 *            = 0
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive implementation with depth O(log n).  No dynamic allocation needed. */
seq_t a(idx_t n)
{
    if (n < 2)                     /* a(0) = 0, a(1) = 0 */
        return (seq_t)0;

    idx_t m = n >> 1;              /* floor(n/2)  */
    seq_t sub = a(m);              /* a(floor(n/2)) */
    seq_t mm = (seq_t)m;            /* cast to seq_t for arithmetic */

    return sub * 2 + mm * (mm + 1);
}
