/*
 * A339541  a(n) = n + sod(n, sod(n, 10)), where sod(n,b) is the sum of base-b
 *          digits of n.
 * https://oeis.org/A339541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + sod(n, sod(n, 10))
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9953 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sod(n,b) is the sum of the base-b digits of n. */

#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Compute sum of digits of value in base `base`.
   For base 1, the base‑1 representation is unary, so the sum is `value` itself. */
static inline seq_t sum_digits_base(seq_t value, seq_t base) {
    if (base <= 1) return value;
    seq_t sum = 0;
    while (value > 0) {
        sum += value % base;
        value /= base;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Convert the index to the sequence type early to avoid mixed‑type arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* First sum of decimal digits (base 10). */
    seq_t dsum = digit_sum(n_seq);

    /* Sum of digits of n in base equal to the decimal digit sum. */
    seq_t bsod = sum_digits_base(n_seq, dsum);

    return n_seq + bsod;
}
