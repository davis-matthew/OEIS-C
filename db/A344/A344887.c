/*
 * A344887  a(n) is the least base k >= 2 that the base-k digits of n are
 *          nonincreasing.
 * https://oeis.org/A344887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least base k ≥ 2 such that the base‑k digits of n are
 *            nonincreasing
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9217 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline bool is_nonincreasing(uint64_t n, uint64_t base)
{
    if (n < base)   /* single digit */
        return true;
    uint64_t digits[64];   /* enough for 64-bit numbers in base 2 */
    int len = 0;
    uint64_t t = n;
    while (t != 0) {
        digits[len++] = t % base;
        t /= base;
    }
    /* check from most significant towards least */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] < digits[i - 1])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* special handling for n = 0 and 1 */
    if (n <= 1)
        return (seq_t)2;

    /* search from base 2 upward */
    uint64_t n64 = (uint64_t)n;    /* n fits in uint64_t */
    for (uint64_t b = 2; b <= n64; ++b) {
        if (is_nonincreasing(n64, b))
            return (seq_t)b;
    }
    /* fallback (should not be reached, base n always works) */
    return (seq_t)n64;
}
