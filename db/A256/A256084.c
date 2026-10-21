/*
 * A256084  Non-palindromic balanced numbers in base 4.
 * https://oeis.org/A256084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑palindromic balanced integer in base 4
 * Range:     u8 n <= 5, u16 n <= 1947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced_nonpal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number is balanced if ∑(digit_i · (i − (L−1)/2)) = 0, where i is the
 * position from the left (0 = most‑significant) and L is the number of digits.
 * The equivalent integer test is ∑(digit_i · (2·i_left − (L−1))) = 0,
 * with i_left = L−1−i. The number must not read the same forwards and
 * backwards. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/balanced_nonpal.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;           /* number of balanced non‑palindromic numbers found */
    idx_t candidate = 0;       /* start checking from 1 */

    for (;;) {
        ++candidate;
        if (!balanced_nonpal(candidate))
            continue;
        ++count;
        if (count == n)
            return (seq_t)candidate;
    }
}
