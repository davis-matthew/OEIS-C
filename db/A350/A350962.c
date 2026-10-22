/*
 * A350962  a(n) = A068527(2*n).
 * https://oeis.org/A350962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A068527(2*n) – computed using an integer‑sqrt based *
 *            piecewise expression as described in the OEIS. The result is *
 *            returned in seq_t
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit value.
 * Returns floor(sqrt(n)).  Operates solely with integer arithmetic. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    /* Largest power of four <= 2^64 */
    idx_t bit = 1ULL << 62;
    while (bit > n) bit >>= 2;            /* find the highest usable bit   */
    for (; bit; bit >>= 2) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute k = floor(sqrt(n / 2)) with integer arithmetic. */
    idx_t half = n >> 1;                  /* floor(n/2) */
    idx_t k = isqrt_u64(half);

    /* double_k_sq = 2 * k * k */
    idx_t double_k_sq = (k * k) * 2ULL;

    /* Exact zero value for perfect double squares. */
    if (double_k_sq == n)
        return 0;

    /* Offset inside the current block. */
    idx_t s = n - double_k_sq;            /* 1 ≤ s ≤ 4k + 1 */

    seq_t result;
    if (s <= (2 * k)) {                   /* odd part of the block   */
        result = (seq_t)((4 * k + 1) - (2 * s));
    } else {                              /* even part of the block  */
        result = (seq_t)((8 * k + 4) - (2 * s));
    }

    return result;
}
