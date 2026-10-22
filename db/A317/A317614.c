/*
 * A317614  a(n) = (1/2)*(n^3 + n*(n mod 2)).
 * https://oeis.org/A317614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n*(n mod 2))/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;                     /* cast to sequence type */
    seq_t cube = nn * nn * nn;                 /* n^3 */
    seq_t add  = nn * ((seq_t)(n & 1));        /* n * (n mod 2) */
    return (cube + add) / 2;                   /* divide by 2 */
}
