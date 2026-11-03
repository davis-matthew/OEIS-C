/*
 * A270341  Numbers that equal the sum of the squares of the digits of their
 *          own square in base 43.
 * https://oeis.org/A270341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) = the n-th integer that equals the sum of the squares of
 *            the digits of its own square when the digits are taken in base
 *            43
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Return the sum of the squares of the base‑43 digits of n^2. */
static seq_t digit_square_sum_of_square(seq_t n)
{
    seq_t sq = n * n;
    seq_t sum = 0;
    seq_t t = sq;
    while (t) {
        seq_t d = t % 43;
        sum += d * d;
        t /= 43;
    }
    return sum;
}

/* a(idx_t n) – 1‑based indexing (a(1)=0, a(2)=1, …)
   Works for all provided seq_t and idx_t types. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t last = 0;
    for (idx_t cand = 0; found < n; ++cand) {
        seq_t val = (seq_t)cand;
        if (digit_square_sum_of_square(val) == val) {
            ++found;
            last = val;
        }
    }
    return last;
}
