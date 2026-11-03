/*
 * A274612  Numbers not divisible by 10 with at least one zero but no two
 *          adjacent 0's among its decimal digits.
 * https://oeis.org/A274612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 8949 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate integers in increasing order,
   count those that are not divisible by 10, contain at least
   one zero digit, and have no two adjacent zeros.
   Return the n‑th such integer. */
#include <stdbool.h>

static bool has_valid_zero_pattern(seq_t x)
{
    if (x % 10 == 0) return false;          /* divisible by 10 */
    bool has_zero = false;
    bool prev_zero = false;
    seq_t tmp = x;
    while (tmp > 0) {
        seq_t d = tmp % 10;
        if (d == 0) {
            if (prev_zero) return false;    /* two consecutive zeros */
            prev_zero = true;
            has_zero = true;
        } else {
            prev_zero = false;
        }
        tmp /= 10;
    }
    return has_zero;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        candidate++;
        if (has_valid_zero_pattern(candidate))
            cnt++;
    }
    return candidate;
}
