/*
 * A348296  Irregular table T(n, k), n > 0, k = 1..A000120(n), read by rows;
 *          the n-th contains, in ascending order, the distinct powers of 2
 *          summing to n.
 * https://oeis.org/A348296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   a(n) is the n-th element of the sequence obtained by *
 *            concatenating, for each m = 1, 2, …, the powers of two that sum
 *            to m * (i.e. the set bits of m, listed in ascending order).
 *            Thus, if * r is the unique integer such that * * Σ_{i=1}^{r-1}
 *            popcount(i) < n ≤ Σ_{i=1}^{r} popcount(i) * * and offse...
 * Range:     u8 n <= 1024, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r that contains position n */
    idx_t cumulative = 0;
    idx_t r = 1;
    while (1) {
        idx_t bits = __builtin_popcountll(r);   /* number of set bits in r */
        if (cumulative + bits >= n) break;
        cumulative += bits;
        ++r;
    }

    /* Position inside the row (0‑based) */
    idx_t offset = n - cumulative - 1;

    /* Find the offset‑th set bit of r */
    idx_t seen = 0;
    idx_t pos  = 0;
    for (pos = 0; ; ++pos) {
        if (r & (1ULL << pos)) {
            if (seen == offset) {
                return (seq_t)1 << pos;
            }
            ++seen;
        }
    }
}
