/*
 * A352515  Number of weak nonexcedances (parts on or below the diagonal) of
 *          the n-th composition in standard order.
 * https://oeis.org/A352515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of indices j with part_j <= j in the nth
 *            composition * in standard (graded reverse‑lexicographic) order.
 *            The composition is * derived from the binary representation of
 *            n: * 1. list the 1‑bit positions of n (1‑based,
 *            least‑significant first); * 2. prepend 0, then take successive
 *            diff...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* positions of set bits (1‑based) */
    idx_t positions[65];
    size_t num = 0;
    idx_t tmp = n;
    idx_t pos = 1;
    while (tmp) {
        if (tmp & 1) positions[num++] = pos;
        tmp >>= 1;
        pos++;
    }

    /* compute consecutive differences (the parts) */
    idx_t parts[65];
    for (size_t i = 0; i < num; ++i) {
        idx_t prev = (i == 0) ? 0 : positions[i - 1];
        parts[i] = positions[i] - prev;
    }

    /* count weak nonexcedances (parts <= index) */
    idx_t count = 0;
    for (size_t j = 1; j <= num; ++j) {
        idx_t part = parts[num - j];
        if (part <= (idx_t)j) ++count;
    }

    return (seq_t)count;
}
