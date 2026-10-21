/*
 * A263851  Number of 4-ascent sequences of length n that avoid the pattern
 *          00.
 * https://oeis.org/A263851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{min(n,9)} c[i] * C(n-i+9,9) * with c = [1, -9,
 *            39, -99, 168, -192, 152, -82, 27, -4]
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 45, u64 n <= 570,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int c[10] = { 1, -9, 39, -99, 168, -192, 152, -82, 27, -4 };
    signed __int128 total = 0;
    for (idx_t i = 0; i < 10 && i <= n; ++i) {
        idx_t m = n - i + 9;
        unsigned __int128 bin = 1;
        for (int k = 1; k <= 9; ++k) {
            bin = bin * (unsigned __int128)(m - 9 + k);
            bin = bin / k;
        }
        signed __int128 term = (signed __int128)c[i] * (signed __int128)bin;
        total += term;
    }
    return (seq_t)total;
}
