/*
 * A339853  Read one digit d at the time, starting from the first one; extend
 *          S with the smallest multiple of d not yet present in the sequence.
 *          Zeros are not read.
 * https://oeis.org/A339853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339853.
   Start with the list S = [1].  Read the digits of the current S
   (ignoring zeros).  For each digit d read, append to S the
   smallest multiple of d that is not yet present in S.
   The function returns the nth term (1‑based). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* The external types seq_t and idx_t are supplied by the build system. */
static seq_t *seq_vals   = NULL;   /* stored terms            */
static size_t seq_cap   = 0;       /* capacity of seq_vals     */
static size_t seq_len   = 0;       /* number of stored terms   */

static uint8_t *digit_q = NULL;   /* queue of digits to read   */
static size_t q_cap  = 0;         /* capacity of the queue     */
static size_t q_head = 0;         /* head index into digit_q   */
static size_t q_len  = 0;         /* tail index (length)       */

static bool *used      = NULL;   /* used[v] == true if v present */
static size_t used_cap = 0;       /* capacity of used array     */

static seq_t next_mult[10];       /* next multiple to try for digit d (1‑9) */

/* Ensure the capacity of the term array. */
static void ensure_seq_cap(size_t n)
{
    if (seq_cap < n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure the capacity of the digit queue array. */
static void ensure_q_cap(size_t n)
{
    if (q_cap < n) {
        size_t new_cap = q_cap ? q_cap * 2 : 32;
        while (new_cap < n) new_cap <<= 1;
        digit_q = (uint8_t *)realloc(digit_q, new_cap * sizeof(uint8_t));
        q_cap = new_cap;
    }
}

/* Ensure the capacity of the used array up to index x. */
static void ensure_used_cap(size_t x)
{
    if (x + 1 > used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= x + 1) new_cap <<= 1;
        used = (bool *)realloc(used, new_cap * sizeof(bool));
        for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
        used_cap = new_cap;
    }
}

/* Enqueue a single digit d (1‑9) into the digit queue. */
static void enqueue_digit(uint8_t d)
{
    ensure_q_cap(q_len + 1);
    digit_q[q_len++] = d;
}

/* Push the decimal digits of x onto the digit queue (most significant first). */
static void enqueue_digits(seq_t x)
{
    if (x == (seq_t)0) return;            /* zeros are never pushed */
    /* collect digits in reverse order */
    uint8_t tmp[256];
    size_t tlen = 0;
    while (x != (seq_t)0) {
        tmp[tlen++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    /* append them in correct order */
    /* first ensure the queue can hold these additional digits */
    ensure_q_cap(q_len + tlen);
    for (size_t i = 0; i < tlen; ++i)
        digit_q[q_len++] = tmp[tlen - 1 - i];
}

/* Initialise static state on the first call. */
static void init_once(void)
{
    if (seq_vals != NULL) return;   /* already initialised */

    /* start with 1 */
    ensure_seq_cap(1);
    seq_vals[0] = (seq_t)1;
    seq_len = 1;

    /* enqueue its digits */
    ensure_q_cap(1);
    digit_q[0] = 1;
    q_head = 0;
    q_len = 1;

    /* used array */
    ensure_used_cap(1);
    used[1] = true;

    /* initialise next multiples */
    for (int d = 1; d <= 9; ++d) next_mult[d] = (seq_t)d;
}

/* Main function required by the interface. */
seq_t a(idx_t n)
{
    init_once();

    while (seq_len < n) {
        /* get next digit to process */
        if (q_head >= q_len) break;      /* safety, should not happen */
        uint8_t d = digit_q[q_head++];
        if (d == 0) continue;            /* zeros are ignored */

        /* find smallest multiple of d not yet used */
        seq_t cand = next_mult[d];
        while (cand <= (seq_t)used_cap && used[(size_t)cand]) {
            cand += (seq_t)d;
        }
        /* set next multiple for future */
        next_mult[d] = cand + (seq_t)d;

        /* append the new term */
        ensure_seq_cap(seq_len + 1);
        seq_vals[seq_len++] = cand;

        /* mark as used and grow used array if needed */
        ensure_used_cap((size_t)cand);
        used[(size_t)cand] = true;

        /* enqueue digits of the new term for future processing */
        enqueue_digits(cand);
    }

    return seq_vals[n - 1];
}
