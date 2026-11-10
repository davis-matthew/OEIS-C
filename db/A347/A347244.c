/*
 * A347244  Characteristic function of A347242.
 * https://oeis.org/A347244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1; otherwise a(n) = 1 if some k >= 1 has
 *            A006530(A000593^(k)(n)) >= A006530(n), else 0. The code computes
 *            this by iterating the sum of odd divisors, with trial-division
 *            factoring
 * Range:     u8 n <= 1910, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9969 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Factor m >= 1: *lpf = greatest prime factor (1 if m == 1), *sod = sum of odd divisors. */
static void A347244_factor(seq_t m, seq_t *lpf, seq_t *sod)
{
    seq_t l = 1;
    seq_t s = 1;
    while (m % 2 == 0) {
        l = 2;
        m = m / 2;
    }
    for (seq_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % f == 0) {
                m = m / f;
                pw = pw * f;
                term = term + pw;
            }
            s = s * term;
            l = f;
        }
    }
    if (m > 1) {
        s = s * (m + 1);
        l = m;
    }
    *lpf = l;
    *sod = s;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t l0, s, l, s2;
    A347244_factor((seq_t)n, &l0, &s);
    while (true) {
        if (s == 1)
            return 0;
        A347244_factor(s, &l, &s2);
        if (l >= l0)
            return 1;
        s = s2;
    }
}
