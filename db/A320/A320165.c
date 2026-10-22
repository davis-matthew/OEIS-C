/*
 * A320165  Number of integer solutions to a^2 + 2*b^2 + 5*c^2 + 8*d^2 = n.
 * https://oeis.org/A320165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + 2*b^2 + 5*c^2 +
 *            8*d^2 = n
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square‑root for unsigned 64‑bit values, safe for all n. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    unsigned shift = 63 - __builtin_clzll(n);   /* most significant bit */
    idx_t x = 0;
    for (int i = shift; i >= 0; --i) {
        idx_t tmp = x | (1ULL << i);
        unsigned __int128 prod = (unsigned __int128)tmp * tmp;
        if (prod <= n) x = tmp;
    }
    return x;
}

/* Count solutions to a^2 + 2*b^2 + 5*c^2 + 8*d^2 = n */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Iterate over d. 8*d^2 <= n  -> |d| <= sqrt(n/8) */
    idx_t d_max = isqrt_u64(n / 8);
    for (idx_t d = 0; d <= d_max; ++d)
    {
        idx_t d_sq   = d * d;
        idx_t rem1   = n - 8ULL * d_sq;
        unsigned d_fac = (d == 0) ? 1 : 2;   /* multiplicity for d */

        /* Iterate over a. a^2 <= rem1 */
        idx_t a_max = isqrt_u64(rem1);
        for (idx_t a = 0; a <= a_max; ++a)
        {
            idx_t a_sq   = a * a;
            idx_t rem2   = rem1 - a_sq;
            unsigned a_fac = (a == 0) ? 1 : 2;   /* multiplicity for a */

            /* Iterate over b. 2*b^2 <= rem2  -> |b| <= sqrt(rem2/2) */
            idx_t b_max = isqrt_u64(rem2 / 2);
            for (idx_t b = 0; b <= b_max; ++b)
            {
                idx_t b_sq   = b * b;
                idx_t rem3   = rem2 - 2ULL * b_sq;
                if (rem3 % 5ULL != 0ULL) continue;   /* must be 5*c^2 */

                idx_t k   = rem3 / 5ULL;
                idx_t c   = isqrt_u64(k);
                if (c * c != k) continue;           /* c^2 = k */

                unsigned c_fac = (c == 0) ? 1 : 2;
                unsigned b_fac = (b == 0) ? 1 : 2;

                /* Total multiplicity for this (|a|,|b|,|c|,|d|) */
                unsigned mult = d_fac * a_fac * b_fac * c_fac;
                count += (seq_t)mult;
            }
        }
    }

    return count;
}
