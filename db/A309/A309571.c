/*
 * A309571  Start with a(1)=1; thereafter the sequence is always extended by
 *          adding the n-th digit of the sequence to a(n) if both are of the
 *          same parity, otherwise subtracting it.
 * https://oeis.org/A309571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 361, i16 n <= 9929, i32 n <= 9929, i64 n <= 9929,
 *            i128 n <= 9929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1.
 * For n>=1 let d_n be the n-th decimal digit of the infinite digit string obtained by
 * concatenating the terms a(1),a(2),… .
 * Then
 *   a(n+1) = a(n) + d_n  if a(n) and d_n have the same parity
 *          = a(n) - d_n  otherwise
 * This function memoizes intermediate terms to answer arbitrary queries in O(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of the required integer types */
typedef uint64_t idx_t;   /* provided by the build system */

/* ---------- Global memoization storage ---------- */
static seq_t *seq_cache = NULL;     /* array of computed a(n) values */
static size_t   seq_cap  = 0;       /* allocated capacity of seq_cache   */
static idx_t    seq_len  = 0;       /* number of stored terms             */

static uint8_t *digit_cache = NULL;/* array of concatenated decimal digits */
static size_t   digit_cap   = 0;    /* allocated capacity of digit_cache */
static size_t   digit_len   = 0;    /* number of stored digits            */

/* ----- helpers to grow dynamic arrays ----- */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

static void ensure_digit_capacity(size_t needed)
{
    if (needed <= digit_cap) return;
    size_t new_cap = digit_cap ? digit_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    digit_cache = (uint8_t *)realloc(digit_cache, new_cap);
    digit_cap   = new_cap;
}

/* ----- append the decimal digits of a value to digit_cache ----- */
static void append_digits_of(seq_t val)
{
    seq_t tmp = val;
    if (tmp < 0) tmp = -tmp;  /* assume val may not become negative in sequence */

    uint8_t tmpbuf[128];
    size_t  tlen = 0;

    if (tmp == 0) {
        tmpbuf[tlen++] = 0;
    } else {
        while (tmp > 0) {
            seq_t d = tmp % 10;
            tmp = tmp / 10;
            tmpbuf[tlen++] = (uint8_t)d;
        }
    }

    size_t needed = digit_len + tlen;
    if (needed > digit_cap) ensure_digit_capacity(needed);

    for (size_t i = 0; i < tlen; ++i)
        digit_cache[digit_len++] = tmpbuf[tlen - 1 - i];
}

/* ---------- required exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* out of bounds, not used in OEIS offset 1 */

    /* initialize memoization tables on first call */
    if (seq_cache == NULL) {
        ensure_seq_capacity(64);
        seq_cache[0] = 1;     /* a(1) */
        seq_len = 1;

        ensure_digit_capacity(64);
        digit_len = 0;
        append_digits_of(1);  /* digits of a(1) */
    }

    /* extend stored terms up to n */
    if (n > seq_len) {
        for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
            seq_t prev  = seq_cache[idx - 2];
            uint8_t dgt = digit_cache[idx - 2];
            seq_t next = ((prev & 1) == (dgt & 1)) ? prev + dgt : prev - dgt;

            ensure_seq_capacity(idx);
            seq_cache[idx - 1] = next;

            append_digits_of(next);
        }
        seq_len = n;
    }

    return seq_cache[n - 1];
}
