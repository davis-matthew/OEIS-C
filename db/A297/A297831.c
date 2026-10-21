/*
 * A297831  Solution of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n - 1, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) =
 *          4, and (b(n)) is the increasing sequence of positive integers not
 *          in (a(n)). See Comments.
 * https://oeis.org/A297831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297831: a(n) = a(1)*b(n-1) - a(0)*b(n-2) + 2*n - 1
   with a(0)=1,a(1)=2,b(0)=3,b(1)=4, and (a) and (b) the increasing complementary
   sequences of positive integers. */

/* The function also builds b(n) by using the smallest unused integer. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* External linkage required */
seq_t a(idx_t n);

/* ----------   internal tables   ---------- */
static seq_t *a_arr   = NULL;   /* values of a() */
static seq_t *b_arr   = NULL;   /* values of b() */
static uint8_t *used  = NULL;   /* bitmap of used integers */
static size_t a_cap   = 0;
static size_t b_cap   = 0;
static size_t used_cap= 0;
static idx_t  max_gen = 1;      /* largest computed index of a() */
static bool   initialized = false;
static seq_t next_candidate = 5;/* next integer to test for being free */

/* ----------   helpers for dynamic arrays   ---------- */
static void ensure_used_cap(seq_t val) {
    size_t v = (size_t)val;
    if (v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= v) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        if (used_cap < new_cap)
            memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

static void ensure_a_cap(idx_t n) {
    if ((size_t)n >= a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        a_cap = new_cap;
    }
}

static void ensure_b_cap(idx_t n) {
    if ((size_t)n >= b_cap) {
        size_t new_cap = b_cap ? b_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b_arr = tmp;
        b_cap = new_cap;
    }
}

/* ----------   initial setup   ---------- */
static void init_sequence(void) {
    a_cap = 4;
    a_arr = (seq_t *)calloc(a_cap, sizeof(seq_t));
    b_cap = 4;
    b_arr = (seq_t *)calloc(b_cap, sizeof(seq_t));
    used_cap = 128;
    used  = (uint8_t *)calloc(used_cap, sizeof(uint8_t));

    if (!a_arr || !b_arr || !used) abort();

    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    b_arr[0] = (seq_t)3;
    b_arr[1] = (seq_t)4;

    used[(size_t)1] = 1;
    used[(size_t)2] = 1;
    used[(size_t)3] = 1;
    used[(size_t)4] = 1;

    next_candidate = 5;
    max_gen = 1;
    initialized = true;
}

/* ----------   main sequence function   ---------- */
seq_t a(idx_t n) {
    if (!initialized) init_sequence();

    if (n <= max_gen) return a_arr[n];

    while (max_gen < n) {
        idx_t i = max_gen + 1;          /* compute next index */

        /* enlarge arrays if necessary */
        ensure_a_cap(i);
        ensure_b_cap(i);

        /* a(i) = a(1)*b(i-1) - a(0)*b(i-2) + 2*i - 1 */
        seq_t term = (seq_t)2 * b_arr[i-1] - b_arr[i-2] + (seq_t)2 * (seq_t)i - 1;
        a_arr[i] = term;

        /* mark the term as used */
        ensure_used_cap(term);
        used[(size_t)term] = 1;

        /* find smallest unused integer for b(i) */
        while (true) {
            ensure_used_cap(next_candidate);
            if (!used[(size_t)next_candidate]) break;
            next_candidate++;
        }
        seq_t bi = next_candidate;
        b_arr[i] = bi;
        used[(size_t)bi] = 1;
        next_candidate++;

        max_gen = i;
    }
    return a_arr[n];
}
