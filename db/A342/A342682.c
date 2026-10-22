/*
 * A342682  a(1) = 1; for n > 0: a(2*n) = -a(n+1), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A342682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; a(2)=0; a(2*n) = -a(n+1); a(2*n+1) = a(n)+a(n+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)0;

    /* even indices */
    if ((n & (idx_t)1) == (idx_t)0) {
        idx_t m = n >> 1;          /* m = n/2 */
        seq_t val = a(m + (idx_t)1);/* recursive call for a(m+1) */
        return -val;                /* a(2*m) = -a(m+1) */
    }

    /* odd indices */
    idx_t m = n >> 1;              /* m = floor(n/2) */
    seq_t val1 = a(m);            /* a(n-1)/2 */
    seq_t val2 = a(m + (idx_t)1); /* a((n+1)/2) */
    return val1 + val2;           /* a(2*m+1) = a(m)+a(m+1) */
}
