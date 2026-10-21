/*
 * A257628  Expansion of 1 - f(-x) in powers of x where f() is a Ramanujan
 *          theta function.
 * https://oeis.org/A257628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; for n > 0, a(n) = -c_n where c_n is the coefficient of
 *            (1 - x)(1 - x^2)... . Equivalently, a(n) = 0 unless n is a
 *            generalized pentagonal number n = k(3k∓1)/2 (k>0); then a(n) =
 *            (-1)^{k+1}
 * Range:     i8 n <= 3078, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(0) = 0 by definition */
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t g1 = (kk * (((seq_t)3 * kk) - 1)) / 2;
        if (g1 == sn) return (k & 1) ? (seq_t)1 : (seq_t)(-1);
        if (g1 > sn) break;         /* both g1 and g2 will exceed sn afterward */
        seq_t g2 = (kk * (((seq_t)3 * kk) + 1)) / 2;
        if (g2 == sn) return (k & 1) ? (seq_t)1 : (seq_t)(-1);
        if (g2 > sn) break;         /* subsequent numbers will be larger */
    }
    return 0;                       /* n not a generalized pentagonal number */
}
