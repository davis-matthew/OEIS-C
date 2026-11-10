/*
 * A345112  a(n) is the number of steps to reach a palindrome > n under
 *          repeated applications of the map x -> A345111(x) starting with n,
 *          or 0 if no palindrome is ever reached.
 * https://oeis.org/A345112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k >= 1 such that T^k(n) is a palindrome, where T(x)
 *            = A345111(x) = x + (x with its decimal digits rotated left by
 *            one, leading zeros dropped); 0 if none is found. Computed by
 *            direct iteration on a decimal digit array (search, capped)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A345112_MAX_STEPS 12000

static bool A345112_is_pal(const uint8_t *d, size_t len)
{
    size_t i = 0, j = len - 1;
    while (i < j) {
        if (d[i] != d[j]) return false;
        i++; j--;
    }
    return true;
}

seq_t a(idx_t n)
{
    size_t cap = (size_t)A345112_MAX_STEPS + 64;
    uint8_t *d = (uint8_t *)malloc(cap);
    uint8_t *r = (uint8_t *)malloc(cap);
    if (!d || !r) { free(d); free(r); return 0; }

    /* little-endian decimal digits of n */
    size_t len = 0;
    idx_t t = n;
    while (t > 0) { d[len++] = (uint8_t)(t % 10); t /= 10; }
    if (len == 0) { d[0] = 0; len = 1; }

    seq_t result = 0;
    for (int steps = 1; steps <= A345112_MAX_STEPS; steps++) {
        /* r = rotate-left of d (MSB moves to least significant position) */
        r[0] = d[len - 1];
        for (size_t i = 1; i < len; i++) r[i] = d[i - 1];
        /* d = d + r */
        unsigned carry = 0;
        for (size_t i = 0; i < len; i++) {
            unsigned s = (unsigned)d[i] + (unsigned)r[i] + carry;
            d[i] = (uint8_t)(s % 10);
            carry = s / 10;
        }
        if (carry) d[len++] = (uint8_t)carry;
        while (len > 1 && d[len - 1] == 0) len--;

        if (A345112_is_pal(d, len)) { result = (seq_t)steps; break; }
        if (len + 2 >= cap) break;
    }
    free(d);
    free(r);
    return result;
}
