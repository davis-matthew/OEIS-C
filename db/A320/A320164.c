/*
 * A320164  Number of integer solutions to a^2 + 2*b^2 + 5*c^2 + 7*d^2 = n.
 * https://oeis.org/A320164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of a^2 + 2*b^2 + 5*c^2 +
 *            7*d^2 = n
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) for 64‑bit unsigned values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;      /* largest power of two <= 2^62 */
    while (bit > x) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    idx_t max_a = isqrt_u64(n);

    for (idx_t ai = 0; ai <= max_a; ++ai) {
        idx_t a2 = ai * ai;
        idx_t rem1 = n - a2;

        idx_t max_b = isqrt_u64(rem1 / 2);

        for (idx_t bi = 0; bi <= max_b; ++bi) {
            idx_t b2 = bi * bi;
            idx_t rem2 = rem1 - ((idx_t)2 * b2);

            idx_t max_c = isqrt_u64(rem2 / 5);

            for (idx_t ci = 0; ci <= max_c; ++ci) {
                idx_t c2 = ci * ci;
                idx_t rem3 = rem2 - ((idx_t)5 * c2);

                if (rem3 % 7 != 0) continue;

                idx_t d2 = rem3 / 7;
                idx_t d   = isqrt_u64(d2);
                if (d * d != d2) continue;

                /* Count sign combinations:
                   each of ai, bi, ci contributes 1 if zero, otherwise 2.
                   d contributes 1 if zero, otherwise 2. */
                unsigned combos = (ai == 0 ? 1 : 2) *
                                  (bi == 0 ? 1 : 2) *
                                  (ci == 0 ? 1 : 2);
                unsigned dsign  = (d  == 0 ? 1 : 2);

                count += (seq_t)(combos * dsign);
            }
        }
    }
    return count;
}
