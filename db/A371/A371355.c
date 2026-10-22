/*
 * A371355  Minimum diameter of a Cayley graph on the cyclic group Z_n with
 *          two generators.
 * https://oeis.org/A371355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((sqrt(2*n-1)-1)/2)
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t floor_sqrt_u128(unsigned __int128 val) {
    /* initial estimate using long double sqrt */
    long double d = (long double)val;
    uint64_t m = (uint64_t)floor(sqrtl(d));
    /* correct possible off‑by‑one errors */
    while ((unsigned __int128)(m + 1) * (m + 1) <= val) ++m;
    while ((unsigned __int128)m * m > val) --m;
    return m;
}

seq_t a(idx_t n) {
    /* compute v = 2*n-1 in 128‑bit to avoid overflow */
    unsigned __int128 v = (unsigned __int128)n * 2 - 1;
    uint64_t m = floor_sqrt_u128(v);          /* floor(sqrt(v)) */
    unsigned __int128 mm = (unsigned __int128)m * m;
    seq_t res;
    if (mm == v) {
        /* perfect square: ceil((m-1)/2) = floor(m/2) */
        res = (seq_t)(m / 2);
    } else {
        /* non‑perfect square */
        res = (seq_t)(((m - 1) / 2) + 1);
    }
    return res;
}
