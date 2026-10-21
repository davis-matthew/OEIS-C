/*
 * A281453  Expansion of f(x, x) * f(x^7, x^11) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A281453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + 2∑_{i≥1} x^{i²}) · (1 +
 *            ∑_{j≥1} (x^{j(9j-2)} + x^{j(9j+2)}))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for idx_t (unsigned 64‑bit). */
static inline idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;          /* largest power of four ≤ 2^63 */
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

/* Sequence a(n) for OEIS A281453. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t result = (seq_t)0;

    /* Contribution from squares (i²).  Add 2 if n is a perfect square. */
    idx_t k = isqrt(n);
    if ((idx_t)k * k == n) result += (seq_t)2;

    /* Contributions from the j terms j(9j±2) and cross with squares. */
    for (idx_t j = 1; ; ++j)
    {
        /* Compute m1 = j(9j-2) and m2 = j(9j+2) using 128‑bit arithmetic. */
        unsigned __int128 mj1 = (unsigned __int128)j * (((unsigned __int128)9 * j) - 2);
        if (mj1 > n) break;                 /* m1 exceeds n – no further j needed */
        idx_t m1 = (idx_t)mj1;

        /* j(9j-2) term itself. */
        if (m1 == n) result += (seq_t)1;

        /* Cross term with squares: i² + m1 = n. */
        else {
            idx_t diff = n - m1;
            idx_t kk = isqrt(diff);
            if ((idx_t)kk * kk == diff) result += (seq_t)2;
        }

        unsigned __int128 mj2 = (unsigned __int128)j * (((unsigned __int128)9 * j) + 2);
        if (mj2 <= n) {
            idx_t m2 = (idx_t)mj2;

            if (m2 == n) result += (seq_t)1;
            else {
                idx_t diff = n - m2;
                idx_t kk = isqrt(diff);
                if ((idx_t)kk * kk == diff) result += (seq_t)2;
            }
        }
    }

    return result;
}
