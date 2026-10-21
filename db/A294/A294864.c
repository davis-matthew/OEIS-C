/*
 * A294864  Solution of the complementary equation a(n) = a(n-2) + b(n-2) + n,
 *          where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A294864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1, a(1) = 2 * b(0) = 3, b(1) = 4 (the second least
 *            unused positive integer) * a(n) = a(n‑2) + b(n‑2) + n (n ≥ 2) *
 *            The sequences a() and b() are increasing and complementary. *
 *            The function memoises values, so calls may appear in any order
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Bit‑set for marks of numbers that have appeared in either a() or b() */
static uint8_t *used_bits = NULL;   /* one bit per integer, 0 … */
static size_t used_bytes = 0;       /* current allocation size in bytes */

/* Ensure that bit for value v can be accessed */
static void ensure_used_capacity(idx_t v)
{
    size_t need_bytes = (v >> 3) + 1U;
    if (need_bytes > used_bytes) {
        size_t new_bytes = used_bytes ? used_bytes << 1 : 64U;
        if (new_bytes < need_bytes) new_bytes = need_bytes;
        uint8_t *tmp = (uint8_t*)realloc(used_bits, new_bytes);
        if (!tmp) abort();
        memset(tmp + used_bytes, 0, new_bytes - used_bytes);
        used_bits = tmp;
        used_bytes = new_bytes;
    }
}

/* Mark value v as used */
static void set_used(idx_t v)
{
    ensure_used_capacity(v);
    used_bits[v >> 3] |= (uint8_t)(1U << (v & 7U));
}

/* Test whether value v has been used */
static int is_used(idx_t v)
{
    if ((size_t)v >= used_bytes * 8U) return 0;
    return (used_bits[v >> 3] >> (v & 7U)) & 1U;
}

/* Arrays storing computed a() and b() values */
static seq_t *a_vals = NULL;
static seq_t *b_vals = NULL;
static size_t a_cap = 0, b_cap = 0;
static idx_t next_index = 0;       /* number of a() values computed so far */
static idx_t next_candidate = 5;   /* next integer to try for a new b() term */

/* Grow value array if needed */
static void ensure_a_capacity(idx_t n)
{
    if ((size_t)n >= a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 8;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_vals = tmp;
        a_cap = new_cap;
    }
}

static void ensure_b_capacity(idx_t n)
{
    if ((size_t)n >= b_cap) {
        size_t new_cap = b_cap ? b_cap << 1 : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 8;
        seq_t *tmp = (seq_t*)realloc(b_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b_vals = tmp;
        b_cap = new_cap;
    }
}

/* Initialise the sequences with their base values */
static void init_sequences(void)
{
    ensure_a_capacity(2);
    ensure_b_capacity(2);

    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;
    b_vals[0] = (seq_t)3;
    b_vals[1] = (seq_t)4;

    /* Mark used numbers for 1,2,3,4 */
    set_used(1);
    set_used(2);
    set_used(3);
    set_used(4);

    next_candidate = 5;   /* next integer to try for b() */
    next_index = 2;       /* two values already computed */
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sequences();
        initialized = 1;
    }

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Compute values up to n if needed */
    while (next_index <= n) {
        idx_t idx = next_index;          /* current index to compute */

        /* Recurrence: a(idx) = a(idx-2) + b(idx-2) + idx */
        seq_t aval_prev2 = a_vals[idx - 2];
        seq_t bval_prev2 = b_vals[idx - 2];
        seq_t a_val = aval_prev2 + bval_prev2 + (seq_t)idx;

        ensure_a_capacity(idx);
        a_vals[idx] = a_val;
        if (a_val <= (seq_t)UINT64_MAX) set_used((idx_t)a_val);

        /* Find least unused integer for b(idx) */
        idx_t cand = next_candidate;
        /* Only test usedness for values that fit in 64‑bit */
        while (cand <= (idx_t)UINT64_MAX && is_used(cand)) cand++;
        seq_t b_val = (seq_t)cand;

        ensure_b_capacity(idx);
        b_vals[idx] = b_val;
        if (cand <= (idx_t)UINT64_MAX) set_used(cand);

        next_candidate = cand + 1U;
        ++next_index;
    }

    return a_vals[n];
}
