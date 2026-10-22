/*
 * A329756  Doubly heptagonal pyramidal numbers.
 * https://oeis.org/A329756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n *(5*n-2) *(n+1) *(5*n^3+3*n^2-2*n+6)
 *            *(25*n^3+15*n^2-10*n-12)/1296
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t N = (seq_t)n;
    seq_t NN = N * N;
    seq_t NNN = NN * N;

    seq_t term1 = N;                     /* n */
    seq_t term2 = 5 * N - 2;             /* 5*n-2 */
    seq_t term3 = N + 1;                 /* n+1 */
    seq_t term4 = 5 * NNN + 3 * NN - 2 * N + 6;
    seq_t term5 = 25 * NNN + 15 * NN - 10 * N - 12;

    seq_t prod = term1;
    prod *= term2;
    prod *= term3;
    prod *= term4;
    prod *= term5;
    prod /= 1296;   /* exactly divisible */

    return prod;
}
