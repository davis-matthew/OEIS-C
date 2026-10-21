/*
 * A282473  Multiples of 9 which cannot be expressed as the difference between
 *          a natural number k and its digit sum s(k).
 * https://oeis.org/A282473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282473
 *
 * a(n) = the nth multiple of 9 that is NOT representable as k - digit_sum(k)
 *        for any natural number k.  The multiples of 9 are tested in
 *        increasing order; the function returns the nth missing one.
 *
 * The algorithm keeps a pointer k to the current k and a counter m for the
 * current multiple of 9.  For every k we compute
 *
 *          f = k - digit_sum(k)
 *
 * (digit_sum is taken in base 10).  Because f is a non‑decreasing
 * function of k, we can advance k until f ≥ m.  If f == m, the multiple is
 * representable; otherwise it is missing.  This yields an O(k_max) algorithm,
 * where k_max is approximately the value of the n-th missing multiple
 * (≈ n·99 for typical n), which is far below the 2 s limit for n ≈ 10 000.
 *
 * Note: for build types where seq_t is small (e.g. 8 or 16 bits), the
 * function is only called for n such that the result fits in seq_t.  The
 * intermediate calculations use idx_t (uint64_t) so there is no overflow.
 * The digit sum is computed directly on idx_t, avoiding any truncation.
 */

#include <stdint.h>

/* Compute the decimal digit sum of an idx_t value. */
static inline idx_t digit_sum_idx(idx_t x)
{
    idx_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    idx_t missing = 0;     /* number of missing multiples found so far */
    idx_t k       = 1;     /* current k to evaluate k - digit_sum(k) */
    idx_t m       = 9;     /* current multiple of 9 under consideration */

    for (;;) {
        idx_t ds = digit_sum_idx(k);
        idx_t f  = k - ds;                    /* f(k) */

        /* Skip k until f(k) >= m. */
        if (f < m) {
            ++k;
            continue;
        }

        /* Now f >= m.  If f == m then m is representable; else it is missing. */
        if (f == m) {
            /* m is representable; move to the next multiple. */
        } else /* f > m */ {
            ++missing;
            if (missing == n) {
                return (seq_t)m;
            }
        }
        /* Advance to next multiple of 9. */
        m += 9;
    }
}
