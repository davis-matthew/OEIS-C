/*
 * A355159  Numbers k such that (fractional part of k^(3/2)) < 1/2.
 * https://oeis.org/A355159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer k with fractional part of k^(3/2) < 1/2
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition is equivalent to: k^3 <= m^2 + m where m = floor_sqrt(k^3). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_satisfying(seq_t k)
{
    seq_t k2 = k * k;          /* k^2  */
    seq_t k3 = k2 * k;         /* k^3  */
    seq_t m  = floor_sqrt(k3); /* ⌊√(k^3)⌋ */
    seq_t rhs = m * m + m;     /* m^2 + m */
    return k3 <= rhs;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = (seq_t)0;

    for (;; ++k) {
        if (is_satisfying(k)) {
            if (count == n) return k;
            ++count;
        }
    }
}
