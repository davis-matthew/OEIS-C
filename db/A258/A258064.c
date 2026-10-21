/*
 * A258064  Number of hands of n points in Spanish dominoes.
 * https://oeis.org/A258064
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  full, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of 7‑stone hands of total value n in a Spanish domino set (double‑sixes).
   The function precomputes all C(28,7)=1,184,040 possible hands once, stores the
   distribution of their dot sums in a static histogram, and returns the count
   for the requested sum.  For n outside the admissible range [15,69] it returns 0. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Histogram of sums from all C(28,7) 7‑stone hands.  Indices 0‑69 are
       sufficient because the minimum possible sum is 15 and the maximum is 69. */
    static uint64_t hist[70] = {0};
    static bool ready = false;

    if (!ready) {
        /* List of dot sums for the 28 unique tiles (i ≤ j, 0 ≤ i,j ≤ 6). */
        const unsigned tile[28] = {
            0, 1, 2, 2, 3, 3, 4, 4, 4,
            5, 5, 5, 6, 6, 6, 6, 7, 7,
            7, 8, 8, 8, 9, 9,10,10,11,12
        };

        /* Enumerate all 7‑tuple combinations of distinct tile indices. */
        for (idx_t i0 = 0; i0 < 28; ++i0)
        for (idx_t i1 = i0 + 1; i1 < 28; ++i1)
        for (idx_t i2 = i1 + 1; i2 < 28; ++i2)
        for (idx_t i3 = i2 + 1; i3 < 28; ++i3)
        for (idx_t i4 = i3 + 1; i4 < 28; ++i4)
        for (idx_t i5 = i4 + 1; i5 < 28; ++i5)
        for (idx_t i6 = i5 + 1; i6 < 28; ++i6) {
            int sum =
                tile[i0] + tile[i1] + tile[i2] +
                tile[i3] + tile[i4] + tile[i5] + tile[i6];
            hist[sum]++;               /* safe: 1,184,040 < 2^64 */
        }
        ready = true;
    }

    if (n < 15u || n > 69u)
        return 0;
    return (seq_t)hist[(idx_t)n];
}
