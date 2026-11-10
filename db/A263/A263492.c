/*
 * A263492  Decimal expansion of the generalized hypergeometric function
 *          3F2(1/2,1/2,3/2 ; 1,2 ; x) at x=1/4.
 * https://oeis.org/A263492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = n-th decimal digit of 3F2(1/2,1/2,3/2; 1,2; 1/4) =
 *            Sum_{k>=0} t(k), t(0) = 1, t(k+1) = t(k) * (2k+1)^2*(2k+3) /
 *            (32*(k+1)^2*(k+2)), summed in base-10^9 fixed point with guard
 *            limbs
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BASE 1000000000ULL

static void big_mul_small(uint32_t *x, size_t len, uint64_t m)
{
    uint64_t carry = 0;
    size_t i = len;
    while (i-- > 0) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % BASE);
        carry = v / BASE;
    }
}

static void big_div_small(uint32_t *x, size_t len, uint64_t d)
{
    uint64_t rem = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t v = rem * BASE + x[i];
        x[i] = (uint32_t)(v / d);
        rem = v % d;
    }
}

static void big_add(uint32_t *s, const uint32_t *t, size_t len)
{
    uint64_t carry = 0;
    size_t i = len;
    while (i-- > 0) {
        uint64_t v = (uint64_t)s[i] + t[i] + carry;
        s[i] = (uint32_t)(v % BASE);
        carry = v / BASE;
    }
}

static bool big_is_zero(const uint32_t *x, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        if (x[i]) return false;
    return true;
}

seq_t a(idx_t n)
{
    size_t frac_digits = (size_t)n + 30;
    size_t L = frac_digits / 9 + 2;   /* fractional limbs */
    size_t len = L + 1;               /* plus integer limb */
    uint32_t *t = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint64_t k = 0;
    unsigned digit;

    t[0] = 1;
    s[0] = 1;
    for (;;) {
        big_mul_small(t, len, 2 * k + 1);
        big_div_small(t, len, 2 * (k + 1));
        big_mul_small(t, len, 2 * k + 1);
        big_div_small(t, len, 2 * (k + 1));
        big_mul_small(t, len, 2 * k + 3);
        big_div_small(t, len, 8 * (k + 2));
        if (big_is_zero(t, len)) break;
        big_add(s, t, len);
        k++;
    }

    if (n == 1) {
        digit = s[0] % 10;
    } else {
        size_t j = (size_t)(n - 2);       /* 0-based fractional digit */
        size_t limb = 1 + j / 9;
        unsigned pos = (unsigned)(j % 9);
        uint32_t v = s[limb];
        unsigned p;
        for (p = 0; p < 8 - pos; p++) v /= 10;
        digit = v % 10;
    }

    free(t);
    free(s);
    return (seq_t)digit;
}
