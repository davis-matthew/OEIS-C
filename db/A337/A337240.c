/*
 * A337240  a(n) is the smallest multiple of n whose decimal representation
 *          contains at least two digits which are the same.
 * https://oeis.org/A337240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest multiple of n that contains at least
 * two identical decimal digits (OEIS A337240). */

#include <stdbool.h>
#include "../utils/digit_counts.h"

/* Return true if x has two or more occurrences of any decimal digit */
static bool has_duplicate(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    for (int d = 0; d < 10; ++d)
        if (cnt[d] >= 2)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;

    /* If n itself already has a repeated digit, it is the answer. */
    if (has_duplicate(base))
        return base;

    /* For n ≥ 10^10 the pigeon‑hole principle guarantees a repeated
     * digit in the decimal representation of n, so n is the answer. */
    if (n >= 10000000000ULL)
        return base;

    /* Search multiples of n until we find one with a repeated digit. */
    for (idx_t k = 2; ; ++k) {
        seq_t m = base * (seq_t)k;
        if (has_duplicate(m))
            return m;
    }
}
