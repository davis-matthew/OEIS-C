/*
 * A393564  a(n) is the number of unit squares in the square lattice, divided
 *          by 8, that have 2 vertices strictly inside and 2 vertices strictly
 *          outside the circle x^2 + y^2 = n.
 * https://oeis.org/A393564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unit squares in the integer lattice that have *
 *            2 vertices strictly inside and 2 vertices strictly outside the
 *            circle * x^2 + y^2 = n, divided by 8. The function scans all
 *            squares whose * vertices lie within a square of side 2·(⌊√n⌋+1).
 *            No closed‑form * expression is known, so th...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit integers. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Range for absolute coordinates that can contain all relevant squares. */
    idx_t lim = floor_isqrt_u64(n) + 1;

    /* Use a larger type for intermediate arithmetic to avoid overflow. */
    uint64_t count = 0;
    uint64_t sn = (uint64_t)n;      /* radius squared in uint64_t */

    /* Scan all candidate squares. */
    for (int64_t i = -(int64_t)lim; i <= (int64_t)lim; ++i) {
        uint64_t ii   = (uint64_t)i * (uint64_t)i;
        uint64_t i1i  = (uint64_t)(i + 1) * (uint64_t)(i + 1);

        for (int64_t j = -(int64_t)lim; j <= (int64_t)lim; ++j) {
            uint64_t jj   = (uint64_t)j * (uint64_t)j;
            uint64_t j1j  = (uint64_t)(j + 1) * (uint64_t)(j + 1);

            uint64_t d[4] = {
                ii + jj,    /* (i,   j)   */
                i1i + jj,   /* (i+1, j)   */
                ii + j1j,   /* (i,   j+1) */
                i1i + j1j   /* (i+1, j+1) */
            };

            /* Simple sort of four distances. */
            for (int a = 0; a < 3; ++a) {
                for (int b = a + 1; b < 4; ++b) {
                    if (d[a] > d[b]) {
                        uint64_t t = d[a];
                        d[a] = d[b];
                        d[b] = t;
                    }
                }
            }

            /* Condition: exactly two vertices inside, two outside. */
            uint64_t min2 = d[1] + 1;
            uint64_t max2 = d[2] - 1;
            if (sn >= min2 && sn <= max2)
                ++count;
        }
    }

    /* Each qualifying square is counted once; divide by 8 per definition. */
    return (seq_t)(count / 8);
}
