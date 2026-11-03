/*
 * A381863  Number of triples of triangles that are pairwise edge-disjoint in
 *          the complete graph K_n.
 * https://oeis.org/A381863
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*binomial(n, 6) + 735*binomial(n, 7) + 840*binomial(n,
 *            8) + 280*binomial(n, 9)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 27, u64 n <= 307, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 920 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;
    seq_t res = 0;
    res += (seq_t)120 * binomial(n, 6);
    res += (seq_t)735 * binomial(n, 7);
    res += (seq_t)840 * binomial(n, 8);
    res += (seq_t)280 * binomial(n, 9);
    return res;
}
