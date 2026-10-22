/*
 * A328408  G.f. A(x) satisfies: A(x) = A(x^2) + x * (1 + 4*x + x^2) / (1 -
 *          x)^4.
 * https://oeis.org/A328408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1)=1; a(2*n) = a(n) + 8*n^3; a(2*n+1) = (2*n+1)^3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1555, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if ((n & 1) == 0) {            /* even index */
        idx_t half = n >> 1;
        seq_t half_val = a(half);  /* recurse on n/2 */
        seq_t half_seq = (seq_t)half;
        seq_t term = half_seq * half_seq;   /* (n/2)^2 */
        term = term * half_seq;             /* (n/2)^3 */
        term = term << 3;                   /* 8 * (n/2)^3 */
        return half_val + term;
    } else {                        /* odd index */
        seq_t n_seq = (seq_t)n;
        return n_seq * n_seq * n_seq;       /* n^3 */
    }
}
