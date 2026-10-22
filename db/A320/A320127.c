/*
 * A320127  Number of integer solutions to a^2 + b^2 + 2*c^2 + 6*d^2 = n.
 * https://oeis.org/A320127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer solutions to a^2 + b^2 + 2*c^2 + 6*d^2 = n
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values. */
static inline idx_t isqrt_u64(uint64_t x)
{
    idx_t r = 0;
    idx_t shift = (idx_t)(1ULL << 62);
    while (shift)
    {
        if (r + shift <= x / (r + shift))
            r += shift;
        shift >>= 1;
    }
    return r;
}

/* Count ordered pairs (a,b) of non‑negative integers with a^2 + b^2 = m.
   The result is the number of integer pairs (±a,±b). */
static seq_t count_pairs_sum_sq(seq_t m)
{
    if (m == (seq_t)0)
        return (seq_t)1;
    uint64_t m64 = (uint64_t)m;           /* m fits in 64 bits (n fits in idx_t) */
    idx_t amax = isqrt_u64(m64);
    seq_t total = (seq_t)0;
    for (idx_t a = 0; a <= amax; ++a)
    {
        uint64_t a2 = (uint64_t)a * (uint64_t)a;
        uint64_t rem = m64 - a2;
        idx_t b = isqrt_u64(rem);
        if ((uint64_t)b * (uint64_t)b == rem)
        {
            unsigned fac_a = (a == 0) ? 1 : 2;
            unsigned fac_b = (b == 0) ? 1 : 2;
            total += (seq_t)(fac_a * fac_b);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    seq_t N     = (seq_t)n64;     /* Not strictly needed, kept for symmetry with other values */
    seq_t result = (seq_t)0;

    /* loop over d */
    idx_t dmax = isqrt_u64(n64 / 6);
    for (idx_t d = 0; d <= dmax; ++d)
    {
        uint64_t d2    = (uint64_t)d * (uint64_t)d;
        uint64_t rem_d = n64 - 6 * d2;
        unsigned fac_d = (d == 0) ? 1 : 2;

        /* loop over c */
        idx_t cmax = isqrt_u64(rem_d / 2);
        for (idx_t c = 0; c <= cmax; ++c)
        {
            uint64_t c2    = (uint64_t)c * (uint64_t)c;
            uint64_t rem_c = rem_d - 2 * c2;
            unsigned fac_c = (c == 0) ? 1 : 2;

            seq_t pairs = count_pairs_sum_sq((seq_t)rem_c);
            result += (seq_t)(fac_d * fac_c) * pairs;
        }
    }
    return result;
}
