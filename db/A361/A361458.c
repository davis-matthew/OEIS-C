/*
 * A361458  Size of the symmetric difference of {1,2,3}, {2,4,6}, ...,
 *          {n,2n,3n}.
 * https://oeis.org/A361458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |⋃_{k=1}^{n} {k, 2k, 3k} mod 2| – size of the symmetric
 *            difference of the sets {k,2k,3k} for k = 1..n. Computation: flip
 *            a bit for each element of every set, count bits set to 1.
 *            Complexity: O(n) time, O(n) auxiliary memory
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Allocate parity array for exponents 1..3n */
    size_t len = (size_t)(3 * n + 1);            /* +1 for index 0 unused */
    uint8_t *parity = (uint8_t *)calloc(len, sizeof(uint8_t));
    if (!parity) return (seq_t)0;                /* fall back on OOM */

    for (idx_t k = 1; k <= n; ++k) {
        /* toggle parity for k, 2k, 3k */
        parity[(size_t)k] ^= 1;
        parity[(size_t)(2 * k)] ^= 1;
        parity[(size_t)(3 * k)] ^= 1;
    }

    seq_t count = 0;
    for (size_t m = 1; m <= 3 * n; ++m)
        if (parity[m]) ++count;

    free(parity);
    return count;
}
