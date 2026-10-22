/*
 * A371808  Exponents k > 0 of powers of 2 such that the decimal expansion of
 *          2^k contains more than one 666 substring (overlapping substrings
 *          are counted as distinct).
 * https://oeis.org/A371808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* a(n) is the n‑th exponent k>0 for which 2^k contains more than one
   overlapping "666" in its decimal expansion. The function computes
   values by incrementally generating powers of two and storing the
   exponents that satisfy the condition. */

static seq_t *terms = NULL;          /* array of found exponents */
static size_t terms_len = 0;         /* number of exponents stored */
static size_t terms_cap = 0;         /* capacity of `terms` array */

static uint8_t *power_digits = NULL; /* decimal digits of current 2^k (LSB first) */
static size_t power_len = 0;         /* number of digits */
static size_t power_cap = 0;         /* capacity of `power_digits` array */
static idx_t current_k = 0;          /* exponent of the power currently stored */

/* Ensure that `terms` has room for at least `need` elements. */
static void ensure_terms_size(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that `power_digits` can hold at least `need` digits. */
static void ensure_power_size(size_t need)
{
    if (need <= power_cap) return;
    size_t new_cap = power_cap ? power_cap << 1 : 32;
    if (new_cap < need) new_cap = need;
    uint8_t *tmp = (uint8_t *)realloc(power_digits, new_cap);
    if (!tmp) abort();
    power_digits = tmp;
    power_cap = new_cap;
}

/* Multiply the current power by 2.  Digits are stored least‑significant first. */
static void power_mul2(void)
{
    uint8_t carry = 0;
    for (size_t i = 0; i < power_len; ++i) {
        uint16_t val = (uint16_t)power_digits[i] * 2 + carry;
        power_digits[i] = (uint8_t)(val % 10);
        carry = (uint8_t)(val / 10);
    }
    if (carry) {
        ensure_power_size(power_len + 1);
        power_digits[power_len] = carry;
        ++power_len;
    }
}

/* Count overlapping occurrences of "666" in the current power.
   The array is little‑endian, so we scan from most significant digit. */
static int count_666(void)
{
    if (power_len < 3) return 0;
    int cnt = 0;
    for (size_t i = power_len; i >= 3; --i) {
        if (power_digits[i-1]   == 6 &&
            power_digits[i-2]   == 6 &&
            power_digits[i-3]   == 6)
        {
            ++cnt;
        }
    }
    return cnt;
}

/* Expand the precomputed list until it contains at least `need` terms. */
static void expand_to(size_t need)
{
    if (!power_digits) {
        power_cap = 32;
        power_digits = (uint8_t *)malloc(power_cap);
        if (!power_digits) abort();
        power_digits[0] = 1; /* 2^0 */
        power_len = 1;
        current_k = 0;
    }

    while (terms_len < need) {
        power_mul2();        /* now power = 2^(current_k+1) */
        ++current_k;

        if (count_666() > 1) {
            ensure_terms_size(terms_len + 1);
            terms[terms_len++] = (seq_t)current_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 */
    expand_to((size_t)n);
    return terms[n-1];
}
