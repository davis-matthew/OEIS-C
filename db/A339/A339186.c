/*
 * A339186  Total number of nonzero squares in the partitions of n into 2
 *          parts.
 * https://oeis.org/A339186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{⌊n/2⌋} (is_square(k) + is_square(n - k))
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count non‑zero squares in all unordered 2‑part partitions of n. */
#include <stdbool.h>

/* Integer (floor) square root for 64‑bit values.
   Uses a bit‑by‑bit method that is safe for all idx_t values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;            /* largest power of 4 ≤ 2^63 */
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
    return r;
}

static inline bool is_square_u64(idx_t x)
{
    if (x == 0) return false; /* non‑zero squares only */
    idx_t r = isqrt_u64(x);
    return ( (unsigned __int128)r * r == (unsigned __int128)x );
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < 2) return 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        if (is_square_u64(k))     ++res;
        if (is_square_u64(n - k)) ++res;
    }
    return res;
}
