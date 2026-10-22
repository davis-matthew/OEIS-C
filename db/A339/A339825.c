/*
 * A339825  Odd bisection of the infinite Fibonacci word A003849.
 * https://oeis.org/A339825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 - (⌊(2n+3)φ⌋ - ⌊(2n+2)φ⌋) (φ = golden ratio)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is 1 iff ⌊(2n+3)φ⌋ - ⌊(2n+2)φ⌋ equals 1, otherwise 0.  */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;                     /* largest 2^p <= sqrt(UINT64_MAX) */
    while (bit > 0) {
        idx_t next = r + bit;
        if ((unsigned __int128)next * next <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Floor(k * φ) using the identity ⌊kφ⌋ = ⌊(k + ⌊k√5⌋)/2⌋. */
static inline idx_t floor_phi_idx(idx_t k)
{
    /* Compute ⌊k√5⌋ as ⌊√(5k²)⌋ */
    idx_t s = isqrt_u64((idx_t)((unsigned __int128)5 * k * k));
    return (k + s) >> 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t k1 = 2 * n + 2;      /* 2n + 2 */
    idx_t k2 = k1 + 1;         /* 2n + 3 */

    idx_t f1 = floor_phi_idx(k1);
    idx_t f2 = floor_phi_idx(k2);

    idx_t diff = f2 - f1;     /* diff is 1 or 2 */
    return (seq_t)(diff == 1 ? 1 : 0);
}
