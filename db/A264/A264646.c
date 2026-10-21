/*
 * A264646  A simple self-describing sequence S: n concatenated with the n-th
 *          digit of S.
 * https://oeis.org/A264646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, digit_n_of_S) where S is the concatenation of
 *            all previous terms a(1)…a(n‑1), digit_n_of_S is the n‑th digit
 *            of S (1‑based). For n = 1, digit_1_of_S is defined to be 1
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoised values of the sequence. */
static seq_t *values = NULL;
static size_t values_len = 0;    /* number of values stored */
static size_t values_cap = 0;    /* capacity of the values array */

/* Digits of the stream S. */
static uint8_t *digits = NULL;
static size_t digits_len = 0;     /* number of digits stored */
static size_t digits_cap = 0;     /* capacity of the digits array */

/* Grow the values array to hold at least 'needed' elements. */
static void ensure_values_cap(size_t needed)
{
    if (values_cap < needed) {
        size_t new_cap = values_cap ? values_cap * 2 : 128;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        values = tmp;
        values_cap = new_cap;
    }
}

/* Grow the digits array to hold at least 'needed' elements. */
static void ensure_digits_cap(size_t needed)
{
    if (digits_cap < needed) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 1024;
        if (new_cap < needed)
            new_cap = needed;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        digits = tmp;
        digits_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n = 0 – return 0 */

    /* Compute successive terms until we have at least n values. */
    while (values_len < (size_t)n) {
        idx_t cur = (idx_t)(values_len + 1);   /* index of the term to compute */

        /* Determine the nth digit of the stream S. */
        uint8_t d;
        if (cur == 1)
            d = 1;                       /* the first digit is defined to be 1 */
        else
            d = digits[(size_t)(cur - 1)];   /* zero‑based index */

        /* Extract the decimal digits of 'cur' (least significant first). */
        idx_t tmp = cur;
        uint8_t nd[32];                  /* cur fits in uint64_t → ≤ 20 digits */
        size_t nd_len = 0;
        do {
            nd[nd_len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp);

        /* Build a(cur) = cur concatenated with d. */
        seq_t result = 0;
        for (size_t i = 0; i < nd_len; ++i) {
            result = result * 10 + (seq_t)nd[nd_len - 1 - i];
        }
        result = result * 10 + (seq_t)d;

        /* Store the result. */
        ensure_values_cap(values_len + 1);
        values[values_len++] = result;

        /* Append the digits of cur and d to the stream S. */
        ensure_digits_cap(digits_len + nd_len + 1);
        for (size_t i = 0; i < nd_len; ++i) {
            digits[digits_len++] = nd[nd_len - 1 - i];
        }
        digits[digits_len++] = d;
    }

    return values[n - 1];
}
