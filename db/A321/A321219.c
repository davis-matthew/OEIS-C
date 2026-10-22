/*
 * A321219  Decimal expansion of 2^(-1074).
 * https://oeis.org/A321219
 *
 * Offset:    -323    (a(-323) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 427, u16 n <= 427, u32 n <= 427, u64 n <= 427,
 *            u128 n <= 427, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321219: Decimal expansion of 2^(-1074).
   a(-323) = 4, a(-322) = 9, ..., a(427) = 5.
   The sequence consists of 751 decimal digits: 323 leading zeros
   followed by the 751 decimal digits of 5^1074. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Big integer representation in base 1 000 000 000 (1e9). */
typedef struct {
    uint32_t *limb;     /* least‑significant limb first */
    size_t len;        /* number of limbs */
} BigInt;

/* Create a big integer from an unsigned 32‑bit value. */
static BigInt make_uint(uint32_t v)
{
    BigInt r;
    r.len = 1;
    r.limb = (uint32_t *)malloc(sizeof(uint32_t));
    r.limb[0] = v;
    return r;
}

/* Free a big integer. */
static void free_bigint(BigInt *x)
{
    free(x->limb);
    x->len = 0;
    x->limb = NULL;
}

/* Multiply two big integers; return a new big integer. */
static BigInt multiply(const BigInt *a, const BigInt *b)
{
    size_t alen = a->len;
    size_t blen = b->len;
    size_t rlen = alen + blen;
    BigInt r;
    r.len = rlen;
    r.limb = (uint32_t *)calloc(rlen, sizeof(uint32_t));
    const uint64_t BASE = 1000000000ULL;    /* 1e9 */

    for (size_t i = 0; i < alen; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < blen; ++j) {
            uint64_t cur = (uint64_t)a->limb[i] * b->limb[j]
                           + r.limb[i + j] + carry;
            r.limb[i + j] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        r.limb[i + blen] = (uint32_t)carry;
    }

    /* remove leading zero limbs */
    while (r.len > 1 && r.limb[r.len - 1] == 0)
        r.len--;

    return r;
}

/* Exponentiation by squaring: base^exp.  base is checked to be < 2^32. */
static BigInt power_uint(uint32_t base, int exp)
{
    BigInt result = make_uint(1);
    BigInt b = make_uint(base);

    while (exp > 0) {
        if (exp & 1) {
            BigInt tmp = multiply(&result, &b);
            free_bigint(&result);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            BigInt tmp = multiply(&b, &b);
            free_bigint(&b);
            b = tmp;
        }
    }
    free_bigint(&b);
    return result;
}

/* Convert a BigInt to a decimal string.
   Caller supplies buffer big enough to hold the digits plus '\0'.
   The function writes exactly len digits to buffer[0..len-1] and returns len. */
static size_t big_to_decimal(const BigInt *x, char *buffer)
{
    size_t pos = 0;
    const uint32_t BASE = 1000000000U;

    /* Process most significant limb first. */
    for (int i = (int)x->len - 1; i >= 0; --i) {
        if (i == (int)x->len - 1) {
            /* most significant limb: no leading zeros. */
            pos += snprintf(buffer + pos, 25, "%u", x->limb[i]);
        } else {
            /* other limbs: exactly 9 digits with leading zeros. */
            pos += snprintf(buffer + pos, 25, "%09u", x->limb[i]);
        }
    }
    buffer[pos] = '\0';
    return pos;
}

/* Prepare the 751‐digit decimal string of 5^1074 once. */
static void init_digits(uint8_t digits[751])
{
    /* 5^1074 */
    BigInt pow5 = power_uint(5, 1074);

    /* Convert to decimal.  5^1074 has 751 digits. */
    char decimal[760];   /* a little room for safety */
    size_t len = big_to_decimal(&pow5, decimal);
    assert(len == 751);  /* length must match OEIS */
    for (size_t i = 0; i < len; ++i)
        digits[i] = (uint8_t)(decimal[i] - '0');

    free_bigint(&pow5);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* static cache of the 751 digits */
    static bool initialized = false;
    static uint8_t digits[751];

    if (!initialized) {
        init_digits(digits);
        initialized = true;
    }

    /* n=-323 corresponds to digits[0]
       n=427   corresponds to digits[750]           */
    int idx = (int)(n + 323);
    if (idx < 0 || idx >= 751)
        return (seq_t)0;   /* undefined beyond the pattern */
    return (seq_t)digits[idx];
}
