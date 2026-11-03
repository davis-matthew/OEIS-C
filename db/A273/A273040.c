/*
 * A273040  Least k >= 2 such that the base-k digits of n are nondecreasing.
 * https://oeis.org/A273040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal k ≥ 2 such that the digits of n in base k are
 *            nondecreasing from most significant to least
 * Range:     u8 n <= 715, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9934 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool digits_nondecreasing(seq_t n, seq_t k)
{
    seq_t temp = n;
    seq_t digits[128];
    size_t len = 0;

    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = temp % k;
            temp = temp / k;
        }
    }

    /* digits[len-1] is the most significant digit */
    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] > digits[i - 2]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; safe fallback */

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 2; k <= n; ++k) {
        if (digits_nondecreasing(n_seq, (seq_t)k))
            return (seq_t)k;
    }

    /* If no base ≤ n works, base = n+1 (single digit) always works. */
    return (seq_t)(n + 1);
}
