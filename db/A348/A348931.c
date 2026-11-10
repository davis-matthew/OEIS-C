/*
 * A348931  Numbers k congruent to 1 or 5 mod 6, for which A348930(k) < k.
 * https://oeis.org/A348931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k == 1 or 5 (mod 6) and A348930(k) < k, where
 *            A348930(k) = sigma(k) with all factors of 3 removed; found by
 *            direct search
 * Range:     u8 n <= 44, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t A348931_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t A348931_A348930_a(idx_t n)
{
    seq_t val = A348931_sigma_of(n);
    while ((val % (seq_t)3) == 0)
        val /= 3;
    return val;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    for (;;) {
        /* next number congruent to 1 or 5 mod 6 */
        if (k == 0) k = 1;
        else if (k % 6 == 1) k += 4;
        else k += 2;
        if (A348931_A348930_a(k) < (seq_t)k) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
