/*
 * A395421  a(n) = (2*n-3-(-1)^n)*(10*n-1+(-1)^n)/8.
 * https://oeis.org/A395421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3-(-1)^n)*(10*n-1+(-1)^n)/8 * Alternative simple
 *            formulas: * a(2*k) = 10*k*(k-1) * a(2*k+1) = 2*k*(5*k+2)
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n == 0 explicitly to avoid underflow in (k-1) */
    if (n == 0)
        return (seq_t)0;

    if ((n & 1) == 0) {           /* even n */
        idx_t k = n >> 1;          /* n = 2*k */
        seq_t kk = (seq_t)k;
        return (seq_t)10 * kk * (kk - 1);
    } else {                       /* odd n */
        idx_t k = (n - 1) >> 1;    /* n = 2*k + 1 */
        seq_t kk = (seq_t)k;
        return (seq_t)2 * kk * ((seq_t)5 * kk + 2);
    }
}
