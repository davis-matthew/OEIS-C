/*
 * A255723  Another variant of Per Nørgård's "infinity sequence", cf. A004718:
 *          t(0) = 0; t(4*n) = t(n); t(4*n+1) = t(n) - 2; t(4*n+2) = -t(n) -
 *          1; t(4*n+3) = t(n) + 2.
 * https://oeis.org/A255723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   t(0)=0; t(4*n) = t(n); t(4*n+1) = t(n) - 2; t(4*n+2) = -t(n) -
 *            1; t(4*n+3) = t(n) + 2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* recurse on the integer division by 4 */
    idx_t q = n >> 2;
    unsigned r = n & 3;           /* r = n mod 4 */
    seq_t tq = a(q);

    switch (r) {
    case 0: return tq;
    case 1: return tq - (seq_t)2;
    case 2: return -tq - (seq_t)1;
    case 3: return tq + (seq_t)2;
    }
    /* unreachable, keep compiler happy */
    return (seq_t)0;
}
