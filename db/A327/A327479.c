/*
 * A327479  a(n) is the minimum number of squares of unit area that must be
 *          removed from an n X n square to obtain a connected figure without
 *          holes and of the longest perimeter.
 * https://oeis.org/A327479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(-25 + 2*n*(2 + n) - 7*cos(n*π) - 4*sin(n*π/2)) for
 *            n > 4; a(0)=0, a(1)=0, a(2)=0, a(3)=4, a(4)=6
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the small cases explicitly, where the closed‑form does not apply. */
    static const seq_t base[5] = {0, 0, 0, 4, 6};
    if (n < 5) {
        return base[n];
    }

    /* Use a 64‑bit unsigned intermediate to avoid overflow for n up to ~1e9. */
    uint64_t prod = (uint64_t)n * ((uint64_t)n + 2);

    if ((n & 1ULL) == 0) {           /* even n */
        prod -= 16;
    } else {
        if ((n & 3ULL) == 1) {       /* n ≡ 1 (mod 4) */
            prod -= 11;
        } else {                     /* n ≡ 3 (mod 4) */
            prod -= 7;
        }
    }

    prod /= 2;                        /* final division by 2 */

    return (seq_t)prod;
}
