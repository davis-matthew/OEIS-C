/*
 * A329030  The least missing nonzero digit in the primorial base expansion of
 *          A276086(n).
 * https://oeis.org/A329030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329028(A276086(n)): least positive integer missing from
 *            the nonzero digits of M = prod prime(k+1)^d_k (d_k =
 *            primorial-base digits of n) written in primorial base; M is held
 *            in a small bignum
 * Range:     u8 n <= 32768, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 122 terms through n = 9924 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A329030_LIMBS 512

static bool A329030_isprime(uint32_t x)
{
    if (x < 2) return false;
    for (uint32_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint32_t big[A329030_LIMBS];
    size_t len = 1;
    big[0] = 1;

    /* build M = A276086(n) */
    uint64_t rem = (uint64_t)n;
    uint32_t p = 2;
    uint64_t base = 2; /* radix for digit k is prime(k+1) */
    while (rem > 0) {
        uint64_t digit = rem % base;
        rem /= base;
        for (uint64_t t = 0; t < digit; ++t) {
            uint64_t carry = 0;
            for (size_t i = 0; i < len; ++i) {
                uint64_t v = (uint64_t)big[i] * p + carry;
                big[i] = (uint32_t)v;
                carry = v >> 32;
            }
            if (carry) big[len++] = (uint32_t)carry;
        }
        /* next prime */
        uint32_t q = p + 1;
        while (!A329030_isprime(q)) ++q;
        p = q;
        base = p;
    }

    /* primorial base expansion of M */
    bool seen[256];
    memset(seen, 0, sizeof(seen));
    uint32_t pr = 2;
    while (!(len == 1 && big[0] == 0)) {
        uint64_t r = 0;
        for (size_t i = len; i-- > 0; ) {
            uint64_t cur = (r << 32) | big[i];
            big[i] = (uint32_t)(cur / pr);
            r = cur % pr;
        }
        while (len > 1 && big[len - 1] == 0) --len;
        if (r != 0 && r < 256) seen[r] = true;
        uint32_t q = pr + 1;
        while (!A329030_isprime(q)) ++q;
        pr = q;
    }

    for (int d = 1; d < 256; ++d)
        if (!seen[d]) return (seq_t)d;
    return (seq_t)256;
}
