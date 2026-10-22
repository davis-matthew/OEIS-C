/*
 * A329510  Expansion of (1 + x)*(1 + x + x^2)*(1 + x^2 - x^3) / (1 - x).
 * https://oeis.org/A329510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n,6)} c_k where c = [1,2,3,2,0,-1,-1] and
 *            a(n) = 6 for n >= 6
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= 6) return (seq_t)6;

    static const int coeff[7] = {1, 2, 3, 2, 0, -1, -1};
    int sum = 0;
    for (idx_t i = 0; i <= n; ++i) sum += coeff[i];
    return (seq_t)sum;
}
