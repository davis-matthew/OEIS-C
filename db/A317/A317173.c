/*
 * A317173  a(n) is the least k > 0 such that k * n contains a digit 1 in its
 *          decimal representation.
 * https://oeis.org/A317173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the smallest k ≥ 1 such that k*n has a decimal digit 1.
   The sequence satisfies 1 ≤ a(n) ≤ 5 for all n. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Return true iff the product k*n (computed without overflowing) contains
   the decimal digit '1'.  We work in decimal, so this works for any n   */
static bool contains_digit_one(idx_t n, int k)
{
    /* Maximum decimal digits needed for 64‑bit n * 5 is 21.  Use a
       fixed buffer that is large enough for all cases. */
    uint8_t digits[64];
    size_t len = 0;

    /* Convert n to decimal digits, least significant first. */
    do {
        digits[len++] = (uint8_t)(n % 10);
        n /= 10;
    } while (n > 0);

    /* Multiply the number by k, still in decimal. */
    int carry = 0;
    for (size_t i = 0; i < len; ++i) {
        int prod = (int)digits[i] * k + carry;
        digits[i] = (uint8_t)(prod % 10);
        carry    = prod / 10;
    }
    while (carry > 0) {
        digits[len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }

    /* Scan for digit 1. */
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 1) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    for (int k = 1; k <= 5; ++k) {
        if (contains_digit_one(n, k))
            return (seq_t)k;
    }
    /* By theory this line is unreachable, but provides a fallback. */
    return (seq_t)5;
}
