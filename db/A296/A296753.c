/*
 * A296753  Numbers whose base-14 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9734 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A296753 by brute‑force enumeration
 * of base‑14 digits: numbers whose number of rises equals number of falls.
 */
#include <stdbool.h>
#include <stddef.h>

/* Return true iff val has an equal number of rises and falls in base 14. */
static bool good_base14(seq_t val)
{
    /* Numbers with fewer than two digits always satisfy the condition. */
    if (val < 14)
        return true;

    /* Store the base‑14 digits from least‑significant to most‑significant. */
    unsigned char digits[64];
    size_t len = 0;
    seq_t t = val;
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 14);
        t /= 14;
    }

    int rises = 0, falls = 0;
    /* Compare successive digits; indices run from 0 (lsd) to len-2. */
    for (size_t i = 0; i + 1 < len; ++i) {
        if (digits[i] < digits[i + 1])
            ++rises;
        else if (digits[i] > digits[i + 1])
            ++falls;
    }
    return rises == falls;
}

/* Main sequence generator: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n = 0; return 0 as a safe default */
        return 0;

    seq_t candidate = 1;
    idx_t count = 0;

    while (true) {
        if (good_base14(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
