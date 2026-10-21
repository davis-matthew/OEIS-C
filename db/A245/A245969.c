/*
 * A245969  The average Wiener index of the set of all fibonacenes with n
 *          hexagons.
 * https://oeis.org/A245969
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 16*n^2 + 6*n + 1 for n >= 3
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    unsigned long long nn = (unsigned long long)n;
    unsigned long long term = 4ULL * nn * nn * nn
                            + 16ULL * nn * nn
                            + 6ULL * nn
                            + 1ULL;
    return (seq_t)term;
}
