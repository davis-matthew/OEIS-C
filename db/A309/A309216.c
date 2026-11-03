/*
 * A309216  a(0)=0; thereafter a(n) = a(n-1)+n if the (n-1)st digit of the
 *          sequence is even, otherwise a(n) = a(n-1)-n.
 * https://oeis.org/A309216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(0) = 0, for n ≥ 1 a(n) = a(n‑1) + n if the (n‑1)th decimal
 *            digit of the sequence is even a(n) = a(n‑1) – n otherwise
 * Range:     i8 n <= 25, i16 n <= 623, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9475 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append the decimal digits of |val| to the global digit buffer.
   The digits are appended in order (most significant first). */
static void append_abs_digits(seq_t val,
                              uint8_t **digits,
                              size_t *dlen,
                              size_t *dcap)
{
    seq_t x = val;
    if (x < (seq_t)0)
        x = -x;

    /* handle the digit 0 */
    if (x == (seq_t)0) {
        if (*dlen + 1 > *dcap) {
            size_t new_cap = (*dcap ? *dcap : 4) * 2;
            *digits = (uint8_t *)realloc(*digits, new_cap * sizeof(uint8_t));
            *dcap = new_cap;
        }
        (*digits)[*dlen] = 0;
        (*dlen)++;
        return;
    }

    /* count digits */
    seq_t tmp = x;
    size_t ndigits = 0;
    while (tmp > (seq_t)0) {
        tmp = tmp / (seq_t)10;
        ndigits++;
    }

    /* temporary buffer for digits in reverse order */
    uint8_t *tmpbuf = (uint8_t *)malloc(ndigits * sizeof(uint8_t));
    size_t i = ndigits;
    while (x > (seq_t)0) {
        seq_t r = x % (seq_t)10;
        x = x / (seq_t)10;
        --i;
        tmpbuf[i] = (uint8_t)r;
    }

    if (*dlen + ndigits > *dcap) {
        size_t new_cap = (*dcap ? *dcap : 4) * 2;
        while (new_cap < *dlen + ndigits)
            new_cap *= 2;
        *digits = (uint8_t *)realloc(*digits, new_cap * sizeof(uint8_t));
        *dcap = new_cap;
    }

    for (i = 0; i < ndigits; ++i)
        (*digits)[*dlen + i] = tmpbuf[i];
    *dlen += ndigits;
    free(tmpbuf);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state for memoised computation */
    static seq_t *values = NULL;       /* a[0..max_computed]          */
    static size_t values_cap = 0;      /* allocated capacity           */
    static idx_t max_computed = 0;     /* largest index computed       */

    static uint8_t *digits = NULL;     /* concatenated digits           */
    static size_t digits_len = 0;      /* number of stored digits       */
    static size_t digits_cap = 0;      /* allocated capacity for digits */

    /* a(0) is the only term that needs no further state */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Initialise on first call */
    if (!values) {
        values_cap = 1;
        values = mem_new(seq_t, values_cap);
        values[0] = (seq_t)0;
        max_computed = 0;

        digits_cap = 8;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        append_abs_digits((seq_t)0, &digits, &digits_len, &digits_cap);
    }

    /* Compute all terms up to n */
    while (max_computed < n) {
        idx_t k = max_computed;   /* last computed index */
        /* k < digits_len guaranteed because every term contributes ≥1 digit */
        uint8_t d = digits[k];
        int even = (d & 1) == 0;

        seq_t step = (seq_t)(k + 1);   /* value of n for the next term */
        seq_t prev = values[k];
        seq_t new_a = even ? (prev + step) : (prev - step);

        /* ensure storage for new_a */
        if (max_computed + 1 >= values_cap) {
            size_t new_cap = values_cap ? values_cap * 2 : 1;
            values = mem_resize(seq_t, values, new_cap);
            values_cap = new_cap;
        }
        values[max_computed + 1] = new_a;
        max_computed++;

        /* append digits of |new_a| to the digit buffer */
        append_abs_digits(new_a, &digits, &digits_len, &digits_cap);
    }

    return values[n];
}
