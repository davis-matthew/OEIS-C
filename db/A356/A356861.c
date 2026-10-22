/*
 * A356861  a(n) is the number of nonzero digits in the product of the first n
 *          numbers not divisible by 5.
 * https://oeis.org/A356861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of non‑zero decimal digits in the * product of
 *            the first n positive integers that are not divisible by 5. * *
 *            The implementation builds the product in an arbitrary‑precision
 *            * big integer (base 1e9) and then counts the digits by
 *            inspecting * each limb. It works for all idx_t and...
 * Range:     u8 n <= 151, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Base 1e9 big‑integer representation */
#define BIGINT_BASE 1000000000U

typedef struct {
    uint32_t *limb;   /* array of limbs, little‑endian */
    size_t len;       /* used limbs */
    size_t cap;       /* allocated capacity */
} bigint_t;

/* Initialize a bigint with value 1 */
static void bigint_init(bigint_t *b)
{
    b->cap = 8;
    b->len = 1;
    b->limb = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    if (!b->limb) abort();           /* allocation failure */
    b->limb[0] = 1;
}

/* Ensure the bigint has at least n capacity */
static void bigint_grow(bigint_t *b, size_t n)
{
    if (b->cap < n) {
        size_t newcap = b->cap;
        while (newcap < n) newcap <<= 1;
        b->limb = (uint32_t *)realloc(b->limb, newcap * sizeof(uint32_t));
        if (!b->limb) abort();
        b->cap = newcap;
    }
}

/* Multiply bigint by a small uint32_t value (m <= 2^32-1). */
static void bigint_mul_small(bigint_t *b, uint32_t m)
{
    uint64_t carry = 0ULL;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limb[i] * m + carry;
        b->limb[i] = (uint32_t)(prod % BIGINT_BASE);
        carry     = prod / BIGINT_BASE;
    }
    while (carry) {
        bigint_grow(b, b->len + 1);
        b->limb[b->len] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
        ++b->len;
    }
}

/* Free the allocated memory of a bigint */
static void bigint_free(bigint_t *b)
{
    free(b->limb);
    b->limb = NULL;
    b->len = b->cap = 0;
}

/* Count non‑zero decimal digits in the bigint. */
static seq_t bigint_nonzero_digit_count(const bigint_t *b)
{
    seq_t count = 0;
    size_t i;

    /* Process all limbs except the most significant one */
    for (i = 0; i + 1 < b->len; ++i) {
        uint32_t limb = b->limb[i];
        /* Each of these limbs contributes exactly 9 decimal digits */
        for (int d = 0; d < 9; ++d) {
            uint32_t digit = limb % 10U;
            if (digit) ++count;
            limb /= 10U;
        }
    }

    /* Process the most significant limb (may have less than 9 digits) */
    uint32_t limb = b->limb[b->len - 1];
    while (limb) {                /* product is never zero */
        uint32_t digit = limb % 10U;
        if (digit) ++count;
        limb /= 10U;
    }

    return count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    bigint_t prod;
    bigint_init(&prod);

    idx_t count = 0;          /* number of terms taken */
    uint64_t m = 1;           /* current integer to consider */

    while (count < n) {
        if (m % 5U == 0U) {   /* skip multiples of 5 */
            ++m;
            continue;
        }
        bigint_mul_small(&prod, (uint32_t)m);
        ++count;
        ++m;
    }

    seq_t result = bigint_nonzero_digit_count(&prod);
    bigint_free(&prod);
    return result;
}
