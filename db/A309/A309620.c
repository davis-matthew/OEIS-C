/*
 * A309620  Start with a(1) = 5. For n>1, the sequence is always extended with
 *          a(n+1) = a(n) + 2*d if d is prime, and with a(n+1) = a(n) - 1 - d
 *          if d is not prime, where d is the n-th digit of the sequence.
 * https://oeis.org/A309620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(1) = 5; for n>1, a(n) = a(n-1) + 2*d if d (the n‑th decimal
 *            digit of the whole sequence) is prime, a(n) = a(n-1) - 1 - d if
 *            d is not prime
 * Range:     i8 n <= 178, i16 n <= 9311, i32 n <= 9311, i64 n <= 9311,
 *            i128 n <= 9311, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global state: memoised terms and the digit stream. */
static seq_t *seq     = NULL;   /* seq[i] holds a(i+1) */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* capacity of seq */

static uint8_t *digits   = NULL;   /* flat array of decimal digits of the sequence */
static size_t  digits_len = 0;     /* number of digits stored */
static size_t  digits_cap = 0;     /* capacity of digits */

/* Ensure seq has at least 'need' slots. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure digits has at least 'need' slots. */
static void ensure_digits_cap(size_t need)
{
    if (need <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    digits = (uint8_t *)realloc(digits, new_cap);
    digits_cap = new_cap;
}

/* Append the decimal digits of v (non‑negative) to the global stream. */
static void append_digits(seq_t v)
{
    seq_t val = v;
    if (val < (seq_t)0) val = -val;  /* values are expected to be non‑negative here */

    uint8_t tmp[64];
    size_t tlen = 0;

    if (val == (seq_t)0) {
        tmp[0] = 0;
        tlen = 1;
    } else {
        while (val > (seq_t)0) {
            unsigned digit = (unsigned)(val % (seq_t)10);
            tmp[tlen++] = (uint8_t)digit;
            val /= (seq_t)10;
        }
    }

    size_t new_len = digits_len + tlen;
    ensure_digits_cap(new_len);

    /* Store digits in most‑significant first order. */
    for (size_t i = 0; i < tlen; ++i)
        digits[digits_len + i] = tmp[tlen - 1 - i];
    digits_len = new_len;
}

/* Test whether a digit is prime (only 2,3,5,7 are prime). */
static inline bool is_prime_digit(uint8_t d)
{
    return d == 2 || d == 3 || d == 5 || d == 7;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safety for out‑of‑bounds */

    /* Initialise storage on first call. */
    if (seq == NULL) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)5;                      /* a(1) */
        seq_len = 1;

        digits_cap = 16;
        digits = (uint8_t *)malloc(digits_cap);
        digits[0] = 5;                          /* first digit */
        digits_len = 1;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* Compute terms up to n. */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        seq_t prev = seq[i - 1];
        uint8_t d   = digits[i - 1];            /* n-th digit (0‑based) */

        seq_t next = is_prime_digit(d) ?
                     prev + (seq_t)(2 * d) :
                     prev - (seq_t)(1 + d);

        ensure_seq_cap(i + 1);
        seq[i] = next;

        append_digits(next);
    }

    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
