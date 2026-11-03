/*
 * A297132  Numbers having a down-first zigzag pattern in base 5; see
 *          Comments.
 * https://oeis.org/A297132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th integer whose base‑5 representation starts with a
 *            down step (first digit > second digit) and contains no equal
 *            consecutive digits. Computed by brute‑force search over integers
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Return true if val has a base‑5 representation whose first comparison is
   down (first digit > second digit) and no two adjacent digits are equal. */
static bool is_valid_down_first(idx_t val)
{
    if (val < 5)       /* requires at least two digits */
        return false;

    uint8_t digits[64];
    size_t len = 0;
    idx_t v = val;
    while (v > 0) {
        digits[len++] = (uint8_t)(v % 5);
        v /= 5;
    }

    /* reverse to obtain most‑significant digit first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i]     = digits[len - 1 - i];
        digits[len-1-i] = tmp;
    }

    /* no consecutive equal digits */
    for (size_t i = 0; i + 1 < len; ++i)
        if (digits[i] == digits[i + 1])
            return false;

    /* first comparison must be down */
    return digits[1] < digits[0];
}

seq_t a(idx_t n)
{
    idx_t hits = 0;
    for (idx_t v = 5; ; ++v) {          /* start from first admissible value */
        if (is_valid_down_first(v)) {
            ++hits;
            if (hits == n)
                return (seq_t)v;          /* cast to seq_t */
        }
    }
}
