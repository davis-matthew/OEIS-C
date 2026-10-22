/*
 * A362740  Dimension of the vector space of 4-invariants on simple 01-labeled
 *          graphs on n vertices.
 * https://oeis.org/A362740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   * a(1)=2, a(2)=5, a(3)=11, a(4)=26, a(5)=58, a(6)=131, a(7)=283
 *            * For n ≥ 8: a(n) = 2·a(n‑1) + n·(n‑1)/2 * (The recurrence holds
 *            for all n ≥ 8; for smaller n the values above are * taken
 *            directly.)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n <= 7 */
    static const seq_t base[] = {0, 2, 5, 11, 26, 58, 131, 283};

    if (n == 0) return 0;            /* undefined, but safe */
    if (n <= 7) return base[n];

    /* Compute iteratively from the last known base value up to n */
    seq_t val = base[7];
    for (idx_t i = 8; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        val = val * 2 + ii * (ii - 1) / 2;
    }
    return val;
}
