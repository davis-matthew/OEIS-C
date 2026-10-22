/*
 * A381565  2-tone chromatic number of a particular class of planar graphs
 *          with 3n+3 vertices.
 * https://oeis.org/A381565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((sqrt(24*n+25)+3)/2) for n <= 6; otherwise a(n) =
 *            ceil((sqrt(24*n+97)+1)/2)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* start with the largest power of 4 ≤ 2^64 */
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

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset is unspecified */

    if (n <= 6) {                         /* first formula */
        idx_t val = 24ULL * n + 25;
        idx_t s   = floor_sqrt(val);
        idx_t res = (s + 3) >> 1;           /* integer division by 2 */
        if (s * s != val) res++;           /* round up if not a perfect square */
        return (seq_t)res;
    } else {                               /* second formula */
        idx_t val = 24ULL * n + 97;
        idx_t s   = floor_sqrt(val);
        idx_t res = (s + 1) >> 1;
        if (s * s != val) res++;
        return (seq_t)res;
    }
}
