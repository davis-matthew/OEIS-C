/*
 * A258440  Number of squares of all sizes in 3*n*(n+1)/2-ominoes in form of
 *          three-quarters of Aztec diamonds.
 * https://oeis.org/A258440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (52*n^3 + 186*n^2 + 212*n -
 *            3*(32*floor(n/3)+3*(1-(-1)^n)))/144
 * Range:     u8 none, u16 n <= 9, u32 n <= 613, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast n to seq_t for arithmetic */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;           /* n^2          */
    seq_t n3 = N * n2;          /* n^3          */
    seq_t term1 = (seq_t)52 * n3;   /* 52*n^3       */
    seq_t term2 = (seq_t)186 * n2;  /* 186*n^2      */
    seq_t term3 = (seq_t)212 * N;   /* 212*n        */

    /* floor(n/3) using integer division */
    seq_t floor_n3 = (seq_t)(n / (idx_t)3);

    /* 1-(-1)^n : 0 for even n, 2 for odd n */
    int parity = (int)(n & 1);   /* 1 if odd, 0 if even */
    int one_minus_pow = 1 - (parity ? -1 : 1); /* 2 if odd, 0 if even */

    /* inner = 32*floor(n/3) + 3*(1-(-1)^n) */
    seq_t inner = (seq_t)32 * floor_n3 + (seq_t)3 * (seq_t)one_minus_pow;
    seq_t term4 = (seq_t)3 * inner;    /* 3 * inner */

    seq_t numerator = term1 + term2 + term3 - term4;
    seq_t result = numerator / (seq_t)144;
    return result;
}
