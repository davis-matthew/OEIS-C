/*
 * A297152  Positive numbers whose decimal expansion contains a 3-digit
 *          substring abc such that b > a > c.
 * https://oeis.org/A297152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal expansion *
 *            contains a three‑digit substring abc such that b > a > c
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x contains a triple of consecutive digits a,b,c
 * (in that order from most significant to least) satisfying b > a > c. */
static bool contains_valid_triple(seq_t x)
{
    /* A number must have at least three decimal digits. */
    if (x < 100)
        return false;

    /* Store digits in reverse order: digits[0] = least significant. */
    unsigned char digits[40];   /* enough for 128‑bit numbers (~39 decimal digits) */
    idx_t len = 0;
    seq_t temp = x;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 10);
        temp /= 10;
    }

    /* Go through each consecutive triple, but the digits are reversed,
     * so the triple a,b,c corresponds to indices (i+2,i+1,i). */
    for (idx_t i = 0; i + 2 < len; ++i) {
        unsigned char a = digits[i + 2];
        unsigned char b = digits[i + 1];
        unsigned char c = digits[i];
        if (b > a && a > c)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num   = 0;

    while (true) {
        ++num;
        if (contains_valid_triple(num)) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
