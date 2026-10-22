/*
 * A333818  G.f.: Sum_{k>=1} x^(k*(3*k - 2)) / (1 - x^(6*k)).
 * https://oeis.org/A333818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that (n/d + 2) mod 3 == 0,
 *            ((n/d + 2)/3) >= d, and parity(((n/d + 2)/3)) == parity(d). This
 *            counts representations of n as a difference of two octagonal
 *            numbers O_j - O_i with j > i >= 0
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    seq_t count = 0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t t_idx = n / d;          /* t = n/d */
            seq_t t = (seq_t)t_idx;
            if ((t + 2) % 3 == 0) {
                seq_t s = (t + 2) / 3;
                seq_t d_seq = (seq_t)d;
                if (s >= d_seq && ((s & 1) == (d_seq & 1)))
                    ++count;
            }
            /* second divisor e = n/d, if distinct */
            idx_t e = n / d;
            if (e != d) {
                idx_t t_idx2 = n / e;      /* t = n/e == d */
                seq_t t2 = (seq_t)t_idx2;  /* actually t2 = d */
                /* but we reuse the same logic with e as divisor */
                seq_t d2 = (seq_t)e;
                /* compute t = n/e */
                seq_t t4 = (seq_t)(n / e);
                if ((t4 + 2) % 3 == 0) {
                    seq_t s2 = (t4 + 2) / 3;
                    if (s2 >= d2 && ((s2 & 1) == (d2 & 1)))
                        ++count;
                }
            }
        }
    }
    return count;
}
