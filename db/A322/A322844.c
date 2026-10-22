/*
 * A322844  a(n) = (1/12)*n^2*(3*(1 + n^2) - 2*(2 + n^2)*(n mod 2)).
 * https://oeis.org/A322844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/12)*n^2*(3*(1 + n^2) - 2*(2 + n^2)*(n mod 2))
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 308, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n2   = (seq_t)n * (seq_t)n;                     /* n^2 */
    seq_t tmp  = 3 * ((seq_t)1 + n2);                    /* 3*(1+n^2) */
    seq_t sub  = 2 * ((seq_t)2 + n2) * (seq_t)(n & 1);   /* 2*(2+n^2)*(n mod 2) */
    seq_t num  = n2 * (tmp - sub);                       /* n^2 * (tmp - sub) */
    return num / 12;                                      /* division gives the term */
}
