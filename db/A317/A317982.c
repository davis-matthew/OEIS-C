/*
 * A317982  Expansion of 14*x*(29 + 784*x + 1974*x^2 + 784*x^3 + 29*x^4) / (1
 *          - x)^7.
 * https://oeis.org/A317982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*n^6 + 210*n^5 + 175*n^4 - 42*n^2 - 7*n (offset 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 19, u64 n <= 800, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t res = 70;
    res = res * x + 210;   // +210*n^5
    res = res * x + 175;   // +175*n^4
    res = res * x;         // +0*n^3
    res = res * x - 42;    // -42*n^2
    res = res * x - 7;     // -7*n
    res = res * x;         // +0 (factor of n^0)
    return res;
}
