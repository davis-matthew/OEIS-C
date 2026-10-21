/*
 * A252669  a(n) is the smallest integer k such that n*k mod (n+k) = 1, or -1
 *          if no such k exists.
 * https://oeis.org/A252669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n*k mod (n+k) = 1, or -1 if no such
 *            k exists. Equivalent to: find the smallest divisor d > n of
 *            n^2+1, then return d-n
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)(-1);   /* outside the defined offset */

    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t m   = nsq + (seq_t)1;        /* n^2+1 */

    /* sqrt_m is the floor of sqrt(m); we will iterate i up to sqrt_m */
    seq_t sqrt_m_seq = floor_sqrt(m);
    if (sqrt_m_seq == (seq_t)(-1)) return (seq_t)(-1); /* safety */
    idx_t sqrt_m = (idx_t)sqrt_m_seq; /* m fits in seq_t, sqrt_m fits in idx_t */

    seq_t best = (seq_t)(-1);          /* sentinel for "not found" */

    for (idx_t i = 1; i <= sqrt_m; ++i) {
        seq_t i_seq = (seq_t)i;
        if (m % i_seq == 0) {
            /* divisor pair: i_seq and d2 = m / i_seq */
            seq_t d1 = i_seq;
            seq_t d2 = m / i_seq;

            if (d1 > (seq_t)n) {
                seq_t k = d1 - (seq_t)n;
                if (best == (seq_t)(-1) || k < best) best = k;
            }
            if (d2 > (seq_t)n && d2 != d1) { /* avoid double‑counting squares */
                seq_t k = d2 - (seq_t)n;
                if (best == (seq_t)(-1) || k < best) best = k;
            }
        }
    }

    return best;      /* Returns the smallest k or -1 if none found */
}
