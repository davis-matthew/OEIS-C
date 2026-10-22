/*
 * A301476  Expansion of (sqrt(8*x^2 - 4*x + 1)*(1 - 4*x))^(-1).
 * https://oeis.org/A301476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = ((32*n+64)*a(n) - (24*n+56)*a(n+1) + (8*n+22)*a(n+2)) /
 *            (n+3) Initial values: a(0)=1, a(1)=6, a(2)=26
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 26;
    seq_t a0 = 1, a1 = 6, a2 = 26;
    for (idx_t k = 3; k <= n; ++k) {
        idx_t m = k - 3;
        seq_t num = ((seq_t)(32 * m + 64)) * a0
                    - ((seq_t)(24 * m + 56)) * a1
                    + ((seq_t)(8 * m + 22)) * a2;
        seq_t a3 = num / (m + 3);   /* m+3 == k */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
