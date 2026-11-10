/*
 * A272684  Terms of A272671 which are not a power of 100 times an earlier
 *          term of A272671.
 * https://oeis.org/A272684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k in A272671 (1k is a perfect square, found by
 *            enumerating squares) such that k is not 100^j*m, j>=1, with m in
 *            A272671
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_square.h"

static bool A272684_inA272671(seq_t m)
{
    seq_t p = 1;
    while (p <= m) p *= 10;          /* p = 10^digits(m) */
    return is_square(p + m);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t s = 4; ; ++s) {
        seq_t t = s * s;
        seq_t p = 1;
        while (p * 10 <= t) p *= 10;
        if (t >= 2 * p) continue;
        seq_t k = t - p;
        if (k < p / 10 || k == 0) continue;
        bool excluded = false;
        seq_t m = k;
        while (m % 100 == 0) {
            m /= 100;
            if (m > 0 && A272684_inA272671(m)) { excluded = true; break; }
        }
        if (excluded) continue;
        ++count;
        if (count == n) return k;
    }
}
