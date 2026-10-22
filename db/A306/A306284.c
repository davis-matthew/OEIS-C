/*
 * A306284  a(n) is the smallest positive integer x such that x > y >= 0 and n
 *          divides x^2 - y^2.
 * https://oeis.org/A306284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A306284:  a(n) = smallest x>y≥0 with n | (x² – y²) */
seq_t a(idx_t n)
{
    seq_t best = (seq_t)n;                 /* upper bound: a(n) ≤ n for n≥1 */

    for (idx_t u = 1; u <= n / u; ++u) {   /* iterate over all divisors u of n */
        if (n % u != 0) continue;
        idx_t m = n / u;                   /* complementary divisor */

        /* ---- pair (u , m) ---- */
        {
            idx_t uu = u;
            idx_t mm = m;

            /* if u is odd while m is even, parity clash → impossible */
            if (!((uu & 1) && !(mm & 1))) {
                idx_t k = uu / mm;
                if (uu % mm) ++k;          /* ceil(uu/mm) */

                /* for odd mm, k must have same parity as uu */
                if (mm & 1) {
                    if ((k & 1) != (uu & 1))
                        ++k;
                }

                idx_t v = mm * k;          /* v = m * k  →  n | (u·v) */
                seq_t x = ((seq_t)uu + (seq_t)v) / 2;
                if (x < best) best = x;
            }
        }

        /* ---- pair (m , u) : use the complementary divisor, if distinct ---- */
        if (m != u) {
            idx_t uu = m;
            idx_t mm = u;

            if (!((uu & 1) && !(mm & 1))) {
                idx_t k = uu / mm;
                if (uu % mm) ++k;          /* ceil(uu/mm) */

                if (mm & 1) {
                    if ((k & 1) != (uu & 1))
                        ++k;
                }

                idx_t v = mm * k;
                seq_t x = ((seq_t)uu + (seq_t)v) / 2;
                if (x < best) best = x;
            }
        }
    }
    return best;
}
