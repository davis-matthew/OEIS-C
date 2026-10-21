/*
 * A294872  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-2) +
 *          b(n-1) + n, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + b(n-1) + n Initial values: a(0)=1,
 *            a(1)=2, b(0)=3 The sequences a() and b() are complementary
 *            increasing integer sequences. The function computes a(n) lazily
 *            with memoisation, supporting arbitrary precision seq_t and
 *            64‑bit idx_t
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Internal storage for sequences and used marks */
static seq_t *a_arr   = NULL;      /* a values */
static size_t a_len   = 0;
static size_t a_cap   = 0;

static seq_t *b_arr   = NULL;      /* b values */
static size_t b_len   = 0;
static size_t b_cap   = 0;

static uint64_t *used_bits = NULL; /* bitmap of used integers */
static size_t used_words   = 0;    /* number of 64‑bit words allocated */

/* smallest integer not yet used */
static idx_t next_candidate = 1;

/* ------- helper macros for bit operations ------- */
#define BIT_WORD(i)   ((i) >> 6)
#define BIT_OFF(i)    ((i) & 63U)
#define BIT_MASK(i)   (1ULL << BIT_OFF(i))

/* ---- dynamic array helpers ---- */
static void ensure_a_capacity(size_t need)
{
    if (a_cap < need) {
        size_t new_cap = a_cap ? a_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
}

static void ensure_b_capacity(size_t need)
{
    if (b_cap < need) {
        size_t new_cap = b_cap ? b_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }
}

static void ensure_used_capacity(idx_t need)
{
    /* need is the largest index that must be addressable (>=0) */
    idx_t bits_needed = need + 1;
    size_t words_needed = (bits_needed + 63) >> 6;
    if (used_words < words_needed) {
        size_t new_cap = used_words ? used_words << 1 : 8;
        if (new_cap < words_needed) new_cap = words_needed;
        used_bits = (uint64_t*)realloc(used_bits, new_cap * sizeof(uint64_t));
        /* zero the new words */
        for (size_t w = used_words; w < new_cap; ++w) used_bits[w] = 0ULL;
        used_words = new_cap;
    }
}

/* Mark an integer value as used, if it fits into 64 bits and is not
   already beyond the current bitmap window. */
static void mark_used(seq_t value)
{
    static seq_t LIM = (seq_t)UINT64_MAX;
    if (value <= LIM) {
        idx_t idx = (idx_t)value;          /* cast is safe because value <= UINT64_MAX */
        ensure_used_capacity(idx);
        used_bits[BIT_WORD(idx)] |= BIT_MASK(idx);
    }
}

/* Init the sequences with the initial values. */
static void init_sequence(void)
{
    /*  a(0)=1, a(1)=2  */
    ensure_a_capacity(2);
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    a_len   = 2;
    a_cap   = 2;

    /*  b(0)=3  */
    ensure_b_capacity(1);
    b_arr[0] = (seq_t)3;
    b_len   = 1;
    b_cap   = 1;

    /* mark used values */
    mark_used(a_arr[0]);
    mark_used(a_arr[1]);
    mark_used(b_arr[0]);

    /* initialise candidate search */
    next_candidate = 1;          /* will advance to 4 on first call */
}

/* Find the smallest positive integer not yet marked used. */
static seq_t find_next_unused(void)
{
    while (1) {
        idx_t cand = next_candidate;
        if (cand >= used_words * 64) return (seq_t)cand;   /* outside current bitmap */
        if (!(used_bits[BIT_WORD(cand)] & BIT_MASK(cand))) return (seq_t)cand;
        ++next_candidate;
    }
}

/* Compute next b value (b(n) for the next index). */
static void compute_next_b(void)
{
    seq_t val = find_next_unused();
    ensure_b_capacity(b_len + 1);
    b_arr[b_len++] = val;
    mark_used(val);

    /* move candidate past the value we just used */
    while (1) {
        ++next_candidate;
        idx_t cand = next_candidate;
        if (cand >= used_words * 64) return;
        if (!(used_bits[BIT_WORD(cand)] & BIT_MASK(cand))) return;
    }
}

/* --------- public function --------- */
seq_t a(idx_t n)
{
    if (a_arr == NULL) init_sequence();

    if (n == 0) return a_arr[0];
    if (n == 1) return a_arr[1];

    /* Lazily compute terms up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* Ensure needed b(i-1) has been computed */
        while (b_len <= i - 1) compute_next_b();

        seq_t prev   = a_arr[i - 1];
        seq_t prev2  = a_arr[i - 2];
        seq_t bj     = b_arr[i - 1];
        seq_t term   = (seq_t)2 * prev - prev2 + bj + (seq_t)i;

        ensure_a_capacity(i + 1);
        a_arr[i] = term;
        mark_used(term);
        a_len = i + 1;
    }

    return a_arr[n];
}
