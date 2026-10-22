/*
 * A339951  Similar to A100787, but alternate with addition and multiplication
 *          instead. See Comments section for more information.
 * https://oeis.org/A339951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1.
   Let D be the next unused decimal digit of the concatenation of all
   previously produced terms (most‑significant digit first).
   For n ≥ 1:
     if (n is odd)   a(n) = a(n-1) + D;
     else            a(n) = a(n-1) * D;
   The digit D is taken sequentially from the pool of digits that grows
   by appending the decimal expansion of each new term. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ----  Internal state --------------------------------------------------- */
static seq_t   *seq_cache = NULL;   /* dynamic array of terms */
static size_t  seq_len   = 0;       /* number of terms stored (next free) */
static size_t  seq_cap   = 0;       /* capacity of seq_cache */

static uint8_t *digit_buf = NULL;   /* pool of decimal digits (MSB first) */
static size_t  digit_size = 0;      /* number of digits stored */
static size_t  digit_cap  = 0;      /* capacity of digit_buf */
static size_t  digit_cursor = 0;    /* index of first unused digit */

static uint8_t *tmp_buf = NULL;     /* temporary buffer for extracting digits */
static size_t  tmp_cap  = 0;        /* capacity of tmp_buf */

/* ----  Helper functions ------------------------------------------------- */
/* Ensure the dynamic arrays are allocated. */
static void init_state(void)
{
    if (seq_len != 0) return;  /* already initialised */

    /* Allocate a cache for a(0)=1 */
    seq_cap = 16;
    seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cache[0] = (seq_t)1;
    seq_len = 1;

    /* Allocate the digit pool and initialise with the digits of 1 */
    digit_cap = 16;
    digit_buf = (uint8_t*)malloc(digit_cap * sizeof(uint8_t));
    if (!digit_buf) abort();
    digit_size = 0;
    digit_cursor = 0;
    digit_buf[digit_size++] = 1;   /* 1 has a single digit 1 */
}

/* Append the decimal digits of x (most‑significant first) into digit_buf. */
static void append_digits(seq_t x)
{
    if (x == (seq_t)0) {
        /* Special case: zero has a single digit 0 */
        if (digit_cap < digit_size + 1) {
            size_t new_cap = digit_cap > 0 ? digit_cap*2 : 16;
            uint8_t *tmp = (uint8_t*)realloc(digit_buf, new_cap*sizeof(uint8_t));
            if (!tmp) abort();
            digit_buf = tmp;
            digit_cap = new_cap;
        }
        digit_buf[digit_size++] = 0;
        return;
    }

    /* Extract digits into tmp_buf in reverse order */
    seq_t t = x;
    size_t count = 0;
    if (tmp_cap == 0) {
        tmp_cap = 16;
        tmp_buf = (uint8_t*)malloc(tmp_cap * sizeof(uint8_t));
        if (!tmp_buf) abort();
    }
    while (t != (seq_t)0) {
        uint8_t d = (uint8_t)(t % (seq_t)10);
        t = t / (seq_t)10;
        if (count == tmp_cap) {
            tmp_cap *= 2;
            uint8_t *nb = (uint8_t*)realloc(tmp_buf, tmp_cap * sizeof(uint8_t));
            if (!nb) abort();
            tmp_buf = nb;
        }
        tmp_buf[count++] = d;      /* store least‑significant first */
    }

    /* Ensure digit_buf has enough space */
    if (digit_cap < digit_size + count) {
        size_t new_cap = digit_cap > 0 ? digit_cap*2 : 16;
        while (new_cap < digit_size + count) new_cap *= 2;
        uint8_t *nb = (uint8_t*)realloc(digit_buf, new_cap * sizeof(uint8_t));
        if (!nb) abort();
        digit_buf = nb;
        digit_cap = new_cap;
    }

    /* Copy digits to digit_buf in correct order (MSB first) */
    for (size_t i = 0; i < count; ++i)
        digit_buf[digit_size++] = tmp_buf[count-1-i];
}

/* ----  Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    init_state();

    /* Compute terms up to n if necessary */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (digit_cursor >= digit_size) abort();  /* no more digits */

        uint8_t d = digit_buf[digit_cursor++];
        seq_t prev = seq_cache[i-1];
        seq_t curr = (i & 1) ? (prev + (seq_t)d) : (prev * (seq_t)d);

        /* Grow the cache if needed */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap > 0 ? seq_cap*2 : 16;
            seq_t *nc = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!nc) abort();
            seq_cache = nc;
            seq_cap = new_cap;
        }
        seq_cache[seq_len++] = curr;

        /* Append new term's digits to the pool */
        append_digits(curr);
    }

    return seq_cache[n];
}
