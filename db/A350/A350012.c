/*
 * A350012  Number of ways to write n as 4*x^4 + y^2 + (z^2 + 4^w)/2 with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A350012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y,z,w) ∈ ℕ⁴ : n = 4*x⁴ + y² + (z² + 4^w)/2}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = ((unsigned __int128)1 << 126);
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (unsigned long long)r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    seq_t count = 0;

    /* 2*n  as unsigned __int128 to avoid overflow for large n   */
    unsigned __int128 twice_n = (unsigned __int128)2 * (unsigned __int128)n;

    /* iterate w: 4^w <= 2*n  */
    unsigned __int128 pow4 = 1;        /* 4^w */
    uint64_t w = 0;
    for (; pow4 <= twice_n; ++w) {
        unsigned __int128 rem1 = twice_n - pow4;   /* 8*x^4 + 2*y^2 + z^2 */

        /* iterate x: 8*x^4 <= rem1 */
        uint64_t x = 0;
        for (;;) {
            unsigned __int128 x2 = (unsigned __int128)x * x;
            unsigned __int128 x4 = x2 * x2;
            unsigned __int128 part_x = x4 * 8;
            if (part_x > rem1) break;

            unsigned __int128 rem2 = rem1 - part_x;   /* 2*y^2 + z^2 */

            /* compute maximum y: 2*y^2 <= rem2   => y <= sqrt(rem2/2)   */
            unsigned __int128 half = rem2 >> 1;
            uint64_t y_max = (uint64_t)isqrt_u128(half);

            for (uint64_t y = 0; y <= y_max; ++y) {
                unsigned __int128 y2 = (unsigned __int128)y * y;
                unsigned __int128 rem3 = rem2 - 2 * y2;     /* z^2 */

                /* compute z and test perfect square */
                unsigned long long z_val = isqrt_u128(rem3);
                if ((unsigned __int128)z_val * z_val != rem3) continue;

                /* parity test: w==0 → z odd; else z even */
                if ((w == 0 && (z_val & 1ULL) == 0) ||
                    (w != 0 && (z_val & 1ULL) == 1)) {
                    continue;
                }

                /* found a valid quadruple */
                count += (seq_t)1;
            }

            ++x;
        }

        pow4 <<= 2;   /* next power: 4^(w+1) */
    }

    return count;
}
