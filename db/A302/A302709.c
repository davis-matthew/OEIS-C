/*
 * A302709  a(n) = Trinomial(2*n+1, 4) = (1/6)*n*(2*n + 1)*(2*n^2 + 9*n + 1),
 *          n >= 0.
 * https://oeis.org/A302709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6) * n * (2*n + 1) * (2*n^2 + 9*n + 1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t two = (seq_t)2;
    seq_t nine = (seq_t)9;
    seq_t six = (seq_t)6;

    seq_t twoNplus1 = two * N + (seq_t)1;
    seq_t inner = two * N * N + nine * N + (seq_t)1;

    seq_t prod = N * twoNplus1 * inner;

    return prod / six;
}
