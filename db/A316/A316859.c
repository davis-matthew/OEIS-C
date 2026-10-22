/*
 * A316859  Triangle read by rows constructed from A076565 as sum of greatest
 *          prime factors.
 * https://oeis.org/A316859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gpf(2*i+1) + gpf(2*j+1) where r is the unique integer
 *            with r*(r+1)/2 >= n, i = n - r*(r-1)/2 (1‑based position in the
 *            row), j = r+1-i
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* find the row r containing the n-th entry */
    idx_t r = 1;
    idx_t tri = 1;            /* r*(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* position i in the row (1‑based) */
    idx_t i = n - ((r - 1) * r) / 2;
    idx_t j = r + 1 - i;      /* mirrored position */

    uint64_t p1 = gpf(2 * i + 1);
    uint64_t p2 = gpf(2 * j + 1);

    return (seq_t)(p1 + p2);
}
