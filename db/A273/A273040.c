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
 * Helpers:   digits_nondecreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digits_nondecreasing.h"

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
