/*
 * A257143  a(2*n) = 3*n if n>0, a(2*n + 1) = 2*n + 1, a(0) = 1.
 * https://oeis.org/A257143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(0)=1; a(2*n)=3*n for n>0; a(2*n+1)=2*n+1
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* base case */
        return (seq_t)1;

    /* even index (n>0): n = 2*k -> a(n) = 3*k */
    if ((n & 1) == 0) {
        seq_t k = ((seq_t)n) >> 1;   /* k = n/2 */
        return k * (seq_t)3;
    }

    /* odd index: a(n) = n */
    return (seq_t)n;
}
