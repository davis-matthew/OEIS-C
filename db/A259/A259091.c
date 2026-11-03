/*
 * A259091  Smallest k such that 2^k contains two adjacent copies of n in its
 *          decimal expansion.
 * https://oeis.org/A259091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k such that 2^k contains two adjacent
 *            copies of the decimal representation of n. This implementation
 *            performs a brute‑force search by repeatedly doubling the value
 *            and scanning the decimal digits for the required pattern
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 938 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void build_pattern(idx_t n, uint8_t **pat, size_t *plen)
{
    char buf[32];
    int l = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)n);
    size_t len = (size_t)l;
    *plen = len * 2;
    *pat = (uint8_t *)malloc(*plen);
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t d = (uint8_t)(buf[i] - '0');
        (*pat)[i] = d;
        (*pat)[len + i] = d;
    }
}

static bool contains_pattern(const uint8_t *digits, size_t dlen,
                             const uint8_t *pat, size_t plen)
{
    if (plen > dlen)
        return false;
    for (size_t i = 0; i + plen <= dlen; ++i)
    {
        bool ok = true;
        for (size_t j = 0; j < plen; ++j)
        {
            if (digits[i + j] != pat[j])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

static void multiply_by_two(uint8_t *digits, size_t *len)
{
    uint8_t carry = 0;
    for (size_t i = *len; i-- > 0; )
    {
        uint16_t tmp = (uint16_t)digits[i] * 2 + carry;
        digits[i] = tmp % 10;
        carry = tmp / 10;
    }
    if (carry)
    {
        for (size_t i = *len; i > 0; --i)
            digits[i] = digits[i - 1];
        digits[0] = carry;
        (*len)++;
    }
}

seq_t a(idx_t n)
{
    uint8_t *pat;
    size_t plen;
    build_pattern(n, &pat, &plen);

    size_t cap = 32;
    uint8_t *digits = (uint8_t *)malloc(cap);
    digits[0] = 1;
    size_t dlen = 1;

    seq_t exponent = 0;

    while (true)
    {
        if (contains_pattern(digits, dlen, pat, plen))
        {
            free(pat);
            free(digits);
            return exponent;
        }

        if (dlen + 1 > cap)
        {
            cap <<= 1;
            digits = (uint8_t *)realloc(digits, cap);
        }

        multiply_by_two(digits, &dlen);
        exponent++;
    }
}
