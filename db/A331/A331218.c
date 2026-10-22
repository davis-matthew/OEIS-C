/*
 * A331218  a(n) is the numbers of ways to write n = u + v where the decimal
 *          representations of u and of v have the same number of digits d for
 *          d = 0..9.
 * https://oeis.org/A331218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ordered pairs (u, v) such that * u + v = n, 0 ≤
 *            u, v ≤ 999 999 999, and the decimal digit * multisets of u and v
 *            are identical. * Leading zeros are ignored. The function is O(n)
 *            per call
 * Range:     u8 n <= 874, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

/* Compare two numbers by their digit frequencies. */
static bool digits_equal(seq_t a, seq_t b)
{
    unsigned char ca[10], cb[10];
    digit_counts(a, ca);
    digit_counts(b, cb);
    for (int i = 0; i < 10; ++i) {
        if (ca[i] != cb[i]) return false;
    }
    return true;
}

/* Counts the number of ways to express n as the sum of two anagrams. */
seq_t a(idx_t n)
{
    const idx_t MAX_DIGIT = 999999999ULL;          /* 9‑digit limit */
    seq_t count = 0;

    for (idx_t u = 0; u <= n; ++u) {
        if (u > MAX_DIGIT) break;                 /* u cannot exceed 9 digits */
        seq_t v = (seq_t)n - (seq_t)u;
        if (v > (seq_t)MAX_DIGIT) continue;       /* v must also be ≤ 9 digits */

        if (digits_equal((seq_t)u, v)) {
            ++count;
        }
    }

    return count;
}
