/*
 * A348933  Numbers k congruent to 1 or 5 mod 6, for which A348930(k^2) < k^2.
 * https://oeis.org/A348933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k == 1 or 5 (mod 6) such that A348930(k^2) <
 *            k^2, where A348930(m) = sigma(m) with all factors of 3 removed;
 *            found by a linear search over k
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A348933_test(seq_t k)
{
    seq_t m = k;
    seq_t sig = 1;
    for (seq_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t term = 1, pw = 1;
            for (int i = 0; i < 2 * e; i++) {
                pw *= p;
                term += pw;
            }
            sig *= term;
        }
    }
    if (m > 1)
        sig *= (1 + m + m * m);
    while (sig % 3 == 0)
        sig /= 3;
    return sig < k * k;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t k = 1;
    for (;;) {
        if (k % 6 == 1)
            k += 4;
        else
            k += 2;
        if (A348933_test(k)) {
            cnt++;
            if (cnt == n)
                return k;
        }
    }
}
