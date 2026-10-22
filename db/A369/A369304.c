/*
 * A369304  Numbers k for which the polynomial (x-1)^3*(x+1)^k has more than
 *          one zero coefficient.
 * https://oeis.org/A369304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((m(n)+3)^2 - 7)/3, where m(n) = floor((3*n-1)/2) is the
 *            n-th positive integer not divisible by 3
 * Range:     u8 n <= 8, u16 n <= 293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n<1, return 0 */
    seq_t nn = (seq_t)n;             /* copy to seq_t for arithmetic */
    seq_t m  = nn + (nn - 1) / 2;    /* m(n) = n + floor((n-1)/2) */
    seq_t t  = m + 3;                /* t = m + 3 */
    seq_t sq = t * t;                /* square */
    seq_t res = (sq - 7) / 3;        /* final formula */
    return res;
}
