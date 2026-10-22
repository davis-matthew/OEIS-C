/*
 * A306645  a(n) is the least positive multiple of n belonging to A306263 if
 *          any, or a(n) = -1 otherwise.
 * https://oeis.org/A306645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive multiple of n belonging to A306263, where
 *            A306263 = {m : for every divisor d of m, popcount(d) divides m}
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Popcount for arbitrary-precision seq_t */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;           /* remove lowest set bit */
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* not defined for n=0, keep safe */
        return (seq_t)-1;

    for (unsigned long long k = 1;; ++k) {
        seq_t m = (seq_t)n * (seq_t)k;          /* candidate multiple */

        bool ok = true;
        /* Iterate divisors i up to sqrt(m) */
        for (idx_t i = 1; ; ++i) {
            seq_t i_t = (seq_t)i;
            /* stop when i^2 > m */
            if (i_t * i_t > m) break;

            /* If i divides m */
            if (m % i == 0) {
                /* first divisor */
                seq_t w = popcount_seq_t(i_t);
                if (w == 0 || m % w != 0) { ok = false; break; }

                /* paired divisor m/i */
                seq_t d2 = m / i_t;
                if (d2 != i_t) {
                    w = popcount_seq_t(d2);
                    if (w == 0 || m % w != 0) { ok = false; break; }
                }
            }
        }

        if (ok)
            return m;
    }
}
