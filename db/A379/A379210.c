/*
 * A379210  List of integers of the form (N^2 - 4)/15.
 * https://oeis.org/A379210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/32)*(30*n^2 - 30*n + 1) + (-1)^n*(5/32)*(2*n - 1) -
 *            (-1)^(n*(n+1)/2)*(1/16)*(6*n - 3 + (-1)^n)
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t N1 = (seq_t)30 * nn * nn - (seq_t)30 * nn + 1;
    int sign_n = ((n & 1) == 0) ? 1 : -1;
    seq_t N2 = (seq_t)sign_n * (seq_t)5 * ((seq_t)2 * nn - 1);
    int mod4   = n & 3;
    int sign3  = (mod4 == 1 || mod4 == 2) ? -1 : 1;
    seq_t inner = (seq_t)6 * nn - 3 + (seq_t)sign_n;
    seq_t N3 = (seq_t)(-sign3) * (seq_t)2 * inner;
    seq_t total = N1 + N2 + N3;
    return total / (seq_t)32;
}
