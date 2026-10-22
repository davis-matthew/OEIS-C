/*
 * A356842  Numbers k such that the k-th composition in standard order does
 *          not cover an interval of positive integers (not gapless).
 * https://oeis.org/A356842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   compose_from_index, not_gapless
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer k such that
 * the k-th composition in the standard order (A066099) has a set of
 * part sizes that is not a contiguous interval of positive integers.
 * The k-th composition is obtained from k by:
 *   * take the positions of 1's in the reversed binary expansion of k
 *     (positions counted from 1 at the least‑significant bit),
 *   * prepend 0,
 *   * take first differences,
 *   * reverse the list of differences.
 * This gives a bijection between nonnegative integers and all
 * compositions.
 *
 * The algorithm below simply enumerates k starting from 1, builds the
 * composition using the above rule, checks whether its part sizes
 * form a contiguous interval, and collects the first n values that
 * do NOT form such an interval.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/compose_from_index.h"
#include "../utils/not_gapless.h"

/* the required function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    idx_t parts[65];
    size_t len;

    while (count < n) {
        compose_from_index(k, parts, &len);
        if (not_gapless(parts, len)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* unreachable, but to silence warnings */
    return (seq_t)0;
}
