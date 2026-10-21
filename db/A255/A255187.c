/*
 * A255187  29-gonal numbers: a(n) = n*(27*n-25)/2.
 * https://oeis.org/A255187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(27*n-25)/2
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t n_t = (seq_t)n;
    /* avoid underflow for n=0 by handling separately.
       For n>0, 27*n_t-25 is positive. */
    if ((n_t & 1) == 0) {
        seq_t half = n_t >> 1;                 /* n/2  (exact) */
        seq_t coeff = ((seq_t)27 * n_t) - (seq_t)25;
        return half * coeff;
    } else {
        seq_t coeff = ((((seq_t)27 * n_t) - (seq_t)25) >> 1);
        return n_t * coeff;
    }
}
