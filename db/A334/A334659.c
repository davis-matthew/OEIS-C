/*
 * A334659  Dirichlet g.f.: 1 / zeta(s-3).
 * https://oeis.org/A334659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = mu(n) * n^3; mu(n) is the Möbius function
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int mu = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
                if (cnt > 1) return 0;
            }
            mu = -mu;
        }
    }
    if (m > 1) mu = -mu;
    seq_t res = ((seq_t)n) * ((seq_t)n) * ((seq_t)n);
    res *= (seq_t)mu;
    return res;
}
