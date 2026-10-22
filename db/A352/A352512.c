/*
 * A352512  Number of fixed points in the n-th composition in standard order.
 * https://oeis.org/A352512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of i such that the i-th part of the composition *
 *            obtained from n by the standard order mapping equals i. * The
 *            mapping is: let P be the list of positions of 1‑bits in n *
 *            (1‑based from the least significant bit). The composition parts
 *            are * d[0] = P[0], d[j] = P[j] – P[j-1] (j ≥ 1)...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Store ascending differences (composition parts) in a small array. */
    uint8_t d[64];
    size_t m = 0;          /* number of parts */
    size_t prev = 0;       /* previous 1‑bit position (1‑based) */

    idx_t tmp = n;
    for (size_t i = 0; tmp; ++i) {
        if (tmp & 1ULL) {
            d[m++] = (uint8_t)(i + 1 - prev);
            prev   = i + 1;
        }
        tmp >>= 1;
    }

    /* Count fixed points in the reversed composition. */
    seq_t count = 0;
    for (size_t j = 0; j < m; ++j) {
        uint8_t part = d[m - 1 - j];
        if (part == (uint8_t)(j + 1)) ++count;
    }
    return count;
}
