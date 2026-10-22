/*
 * A333356  Terms describing the nonprime digits' positions in the way
 *          explained in the Comments section.
 * https://oeis.org/A333356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10*p(n) + d(n) where p(n) is the 1‑based position of the
 *            n-th nonprime digit in the infinite string obtained by
 *            concatenating the terms of this sequence, and d(n) is that digit
 * Range:     u8 n <= 21, u16 n <= 2322, u32 n <= 2322, u64 n <= 2322,
 *            u128 n <= 2322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Maximum bounds for the tests – enough for n up to ~10 000. */
#define MAX_TERMS  20000
#define MAX_DIGITS 200000

/* Static storage of computed terms and the stream of digits. */
static seq_t   terms[MAX_TERMS];
static size_t  terms_len = 0;

static uint8_t digits[MAX_DIGITS];
static size_t  digits_len = 0;

static idx_t next_scan_pos = 1;  /* 1‑based position to start scanning */

/* Test whether a digit is prime (2, 3, 5, 7). */
static int is_prime_digit(uint8_t d)
{
    return d == 2 || d == 3 || d == 5 || d == 7;
}

/* Append the decimal digits of value to the digit stream. */
static void append_digits_of_term(seq_t value)
{
    seq_t  tmp   = value;
    uint8_t buf[32];
    int    cnt   = 0;

    if (tmp == 0) {
        buf[cnt++] = 0;
    } else {
        while (tmp > 0) {
            buf[cnt++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }
    /* buf holds the digits in reverse order; copy them. */
    for (int i = cnt - 1; i >= 0; --i) {
        if (digits_len == MAX_DIGITS) {
            /* Overflow – stop further appends. */
            return;
        }
        digits[digits_len++] = buf[i];
    }
}

/* Generate the next term of the sequence. */
static void generate_next(void)
{
    idx_t pos   = 0;
    uint8_t digit = 0;
    seq_t term;

    if (terms_len == 0) {
        /* The very first term is defined to be 11. */
        pos   = 1;
        digit = 1;
    } else {
        /* Find the next nonprime digit in the accumulated stream. */
        pos = next_scan_pos;
        while (pos <= digits_len) {
            uint8_t d = digits[pos - 1];
            if (!is_prime_digit(d)) {
                digit = d;
                break;
            }
            ++pos;
        }
    }

    term = (seq_t)pos * (seq_t)10 + (seq_t)digit;

    if (terms_len == MAX_TERMS) {
        /* Overflow – ignore further generation. */
        return;
    }
    terms[terms_len++] = term;

    next_scan_pos = pos + 1;
    append_digits_of_term(term);
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Ensure the requested term has been computed. */
    while (terms_len < n) {
        generate_next();
    }
    return terms[n - 1];
}
