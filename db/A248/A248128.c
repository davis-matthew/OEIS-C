/*
 * A248128  Start with a(0)=50, then a(n) = three times the n-th digit of the
 *          sequence, for all n > 0.
 * https://oeis.org/A248128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=50; for n>0, a(n)=3*digit_n where digit_n is the n‑th
   digit of the infinite concatenation of all decimal digits of the terms
   of this sequence, starting with the leftmost digit of a(0). */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for the sequence and for the concatenated digit stream. */
static seq_t *terms = NULL;          /* array of a(i) values */
static size_t terms_len = 0;         /* how many terms are stored */
static size_t terms_cap = 0;         /* allocated capacity of terms */

static uint8_t *digits = NULL;       /* stream of digits of all terms */
static size_t digits_len = 0;        /* how many digits are stored */
static size_t digits_cap = 0;        /* allocated capacity of digits */

static size_t digit_pos = 0;         /* index of next digit to read */

/* Ensure that there is enough room in the terms array. */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap < needed)
    {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* Ensure that there is enough room in the digits array. */
static void ensure_digits_capacity(size_t needed)
{
    if (digits_cap < needed)
    {
        size_t new_cap = digits_cap ? digits_cap * 2 : 256;
        if (new_cap < needed) new_cap = needed;
        digits = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        digits_cap = new_cap;
    }
}

/* Append the decimal digits of a value to the digit stream. */
static void append_digits_of_value(seq_t val)
{
    /* Special‑case zero – its decimal representation is a single digit '0'. */
    if (val == (seq_t)0)
    {
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = 0;
        return;
    }

    /* Collect the digits in reverse order.  A 128‑bit integer has at most
       39 decimal digits; we use 128 to be safe for arbitrary‑precision types. */
    uint8_t tmp[128];
    size_t tmp_len = 0;
    seq_t x = val;
    while (x != (seq_t)0)
    {
        uint8_t d = (uint8_t)(x % 10);
        tmp[tmp_len++] = d;
        x = x / 10;
    }

    /* Now output the digits in the correct order. */
    for (size_t i = tmp_len; i > 0; --i)
    {
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = tmp[i - 1];
    }
}

/* Compute a(n).  The function is fully memoised; it works for arbitrary call
   order and for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* One‑time initialisation of the static cache. */
    if (terms == NULL)
    {
        /* Start with a single term a(0) = 50. */
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 1;
        terms[0] = 50;

        /* Initialise the digit stream with the decimal digits of a(0). */
        digits_cap = 256;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits_len = 2;
        digits[0] = 5;
        digits[1] = 0;

        digit_pos = 0;
    }

    /* Generate terms until we have the requested index. */
    while (terms_len <= (size_t)n)
    {
        /* The next digit must be available. */
        if (digit_pos >= digits_len)
            return (seq_t)0;  /* unexpected: should not happen */

        uint8_t d = digits[digit_pos];
        seq_t val = (seq_t)d * (seq_t)3;  /* a(next) */

        ensure_terms_capacity(terms_len + 1);
        terms[terms_len++] = val;

        append_digits_of_value(val);

        ++digit_pos;
    }

    return terms[(size_t)n];
}
