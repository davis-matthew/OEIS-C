/*
 * A316909  A self-"read and extend" sequence built following the three rules
 *          visible in the Comments section (a variation of A316765).
 * https://oeis.org/A316909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 318, u64 n <= 5989,
 *            u128 n <= 8995, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generated sequence A316909:
   a(1) = 1.
   For n > 1 the terms are produced by reading the decimal digits of the
   sequence itself from left to right and applying the following rules:
   - if the digit is odd: a new term is floor(last_term / 3)
   - if the digit is even and non‑zero: a new term is last_term * 7
   - if the digit is 0: a new term is the smallest non‑negative integer
     not yet present in the sequence.
   The sequence is memoised so that calls to a(n) give the correct term
   independent of the order of the calls.  The implementation simulates
   the above process incrementally. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"

static seq_t *seq = NULL;          /* stored terms, 0‑based */
static idx_t seq_size   = 0;       /* number of terms stored */
static idx_t seq_cap    = 0;

static uint8_t *digit_queue = NULL;   /* FIFO of digits yet to read */
static size_t dhead = 0, dtail = 0, dcap = 0;

static seq_t missing_next = 0;   /* next candidate for “smallest missing” */

/*--- Utilities -------------------------------------------*/

static void seq_Grow(idx_t needed)
{
    if (needed <= seq_cap) return;
    if (seq_cap == 0) seq_cap = 64;
    while (seq_cap < needed) seq_cap <<= 1;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

static void digits_Grow(size_t needed)
{
    if (needed <= dcap) return;
    if (dcap == 0) dcap = 64;
    while (dcap < needed) dcap <<= 1;
    digit_queue = (uint8_t *)realloc(digit_queue, dcap * sizeof(uint8_t));
}

static void digits_Push(uint8_t d)
{
    size_t next = dtail + 1;
    if (next > dcap) digits_Grow(next);
    digit_queue[dtail] = d;
    dtail = next;
}

static int digits_HasMore(void)
{
    return dhead != dtail;
}

static uint8_t digits_Pop(void)
{
    uint8_t d = digit_queue[dhead];
    ++dhead;
    if (dhead == dcap) { dhead = dtail = 0; } /* wrap */
    return d;
}

static void enqueue_digits(seq_t val)
{
    if (val == 0) { digits_Push(0); return; }
    size_t len = (size_t)digit_len(val);
    uint8_t *rev = (uint8_t *)malloc(len);
    seq_t temp = val;
    for (size_t i = 0; i < len; ++i) {
        rev[len - 1 - i] = (uint8_t)(temp % 10);
        temp /= 10;
    }
    for (size_t i = 0; i < len; ++i) digits_Push(rev[i]);
    free(rev);
}

static int value_present(seq_t v)
{
    for (idx_t i = 0; i < seq_size; ++i) {
        if (seq[i] == v) return 1;
    }
    return 0;
}

static seq_t next_missing(void)
{
    seq_t cand = missing_next;
    while (value_present(cand)) ++cand;
    missing_next = cand + (seq_t)1;
    return cand;
}

/*--- Main function ---------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe */
    /* Initialisation on first call */
    if (seq_size == 0) {
        seq_Grow(1);
        seq[0] = (seq_t)1;
        seq_size = 1;
        enqueue_digits(seq[0]);
    }

    while (seq_size < n) {
        /* Read next digit */
        uint8_t d = digits_Pop();
        seq_t next;
        if ((d & 1) == 1) {              /* odd */
            next = seq[seq_size - 1] / (seq_t)3;
        } else if (d == 0) {              /* zero */
            next = next_missing();
        } else {                          /* even non‑zero */
            next = seq[seq_size - 1] * (seq_t)7;
        }

        /* Store new term */
        seq_Grow(seq_size + 1);
        seq[seq_size] = next;
        ++seq_size;

        /* Append digits of new term */
        enqueue_digits(next);
    }

    return seq[n - 1];
}
