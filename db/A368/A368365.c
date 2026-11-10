/*
 * A368365  Numbers n whose comma-number is greater than n.
 * https://oeis.org/A368365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th number k such that A367338(k) - k > k (A367338 =
 *            comma-successor; its comma-number is A367338(k) - k); search
 *            over k <= 99
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A368365_succ(seq_t n_val)
{
    seq_t x = n_val % 10;
    for (seq_t y = 1; y <= 9; ++y) {
        seq_t m = n_val + 10 * x + y;
        seq_t t = m;
        while (t >= 10)
            t /= 10;
        if (t == y)
            return m;
    }
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 1; k <= 99; ++k) {
        seq_t m = A368365_succ(k);
        if (m != 0 && (m - k) > k) {
            ++count;
            if (count == n)
                return k;
        }
    }
    return (seq_t)0;
}
