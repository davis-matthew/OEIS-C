/*
 * A279497  Number of pentagonal numbers dividing n.
 * https://oeis.org/A279497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of pentagonal numbers that divide n, computed by
 *            enumerating pentagonal numbers up to n
 * Range:     u8 n <= 2859, u16 n <= 101269, u32 n <= 101270, u64 n <= 101270,
 *            u128 n <= 101270, big any n
 * Validated: standard, pass, 136 terms through n = 9308 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t count = 0;
    for (idx_t k = 1;; ++k) {
        seq_t k_t = (seq_t)k;
        seq_t Pk = k_t * (k_t * 3 - 1) / 2;  // pentagonal number P(k)
        if (Pk > N) break;
        if (N % Pk == 0) ++count;
    }
    return count;
}
