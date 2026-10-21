/*
 * A297836  Solution of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 3*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          and (b(n)) is the increasing sequence of positive integers not in
 *          (a(n)). See Comments.
 * https://oeis.org/A297836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequence A297836
 * a(n) = a(1) * b(n-1) - a(0) * b(n-2) + 3*n
 * with a(0)=1, a(1)=2, b(0)=3, b(1)=4 and
 * (b(n)) the increasing sequence of positive integers not in (a(n)).
 * This implementation builds the two complementary sequences on demand
 * and caches them so that a(idx_t n) can be called in any order.
 * All memory is dynamically allocated and indices are handled with
 * the supplied integer type idx_t.  The code compiles in both C11
 * and C++17.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- global state ----------------------------------------------------- */
static seq_t *a_cache = NULL;           /* cache of a(n) */
static size_t a_len  = 0;               /* number of computed terms in a_cache */
static size_t a_cap  = 0;               /* capacity of a_cache */

static seq_t *b_cache = NULL;           /* cache of b(n) */
static size_t b_len  = 0;               /* number of computed terms in b_cache */
static size_t b_cap  = 0;               /* capacity of b_cache */

static uint8_t *used = NULL;            /* bitmap of integers already used */
static size_t used_cap = 0;             /* allocated size of used array */
static idx_t next_candidate = 1;        /* smallest integer not yet examined */

/* --- utility functions ----------------------------------------------- */

/* Expand the 'used' bitmap so that index v is valid.  Mark v as being
 * used.  The array is grown exponentially to keep amortised cost small. */
static void mark_used(idx_t v)
{
    if ((size_t)v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 256;
        while (new_cap <= (size_t)v) new_cap *= 2;
        uint8_t *new_used = (uint8_t*)realloc((void*)used, new_cap);
        memset((void*)(new_used + used_cap), 0, new_cap - used_cap);
        used = new_used;
        used_cap = new_cap;
    }
    used[(size_t)v] = 1;
}

/* Return the next unused positive integer, marking it as used.  The
 * global variable next_candidate keeps progress. */
static seq_t get_next_unused(void)
{
    while (1) {
        /* Ensure array covers the current candidate */
        if ((size_t)next_candidate >= used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 256;
            while (new_cap <= (size_t)next_candidate) new_cap *= 2;
            uint8_t *new_used = (uint8_t*)realloc((void*)used, new_cap);
            memset((void*)(new_used + used_cap), 0, new_cap - used_cap);
            used = new_used;
            used_cap = new_cap;
        }
        if (!used[(size_t)next_candidate]) {
            idx_t val = next_candidate;
            used[(size_t)next_candidate] = 1;
            next_candidate++;
            return (seq_t)val;
        }
        next_candidate++;
    }
}

/* Initialise the sequences a and b with the first two terms.  This
 * function is idempotent. */
static void init_sequence(void)
{
    if (a_cache != NULL)  /* already initialised */
        return;

    /* initial capacity for a and b */
    a_cap = 8; a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
    b_cap = 8; b_cache = (seq_t*)malloc(b_cap * sizeof(seq_t));

    /* initial capacity for used bitmap */
    used_cap = 256;
    used = (uint8_t*)calloc(used_cap, sizeof(uint8_t));

    /* a(0) = 1, a(1) = 2 */
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)2;
    mark_used(1);
    mark_used(2);

    /* b(0) = 3, b(1) = 4 */
    b_cache[0] = get_next_unused();   /* 3 */
    b_cache[1] = get_next_unused();   /* 4 */

    a_len = 2;
    b_len = 2;
}

/* Ensure that a_cache and b_cache contain values up to index n
 * inclusive.  The function can be called with n in any order. */
static void ensure_up_to(idx_t n)
{
    init_sequence();                /* guarantee initialisation */

    if ((size_t)n < a_len)          /* already computed */
        return;

    /* Extend arrays if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        a_cache = (seq_t*)realloc((void*)a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
    if ((size_t)n + 1 > b_cap) {
        size_t new_cap = b_cap ? b_cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        b_cache = (seq_t*)realloc((void*)b_cache, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }

    for (size_t idx = a_len; idx <= (size_t)n; ++idx) {
        /* compute a(idx) using the complementary equation */
        seq_t a_val = (seq_t)2 * b_cache[idx-1] - b_cache[idx-2] +
                      (seq_t)3 * (seq_t)idx;
        a_cache[idx] = a_val;
        mark_used((idx_t)a_val);            /* record that a_val is used */

        /* compute b(idx) as the next unused integer */
        seq_t b_val = get_next_unused();
        b_cache[idx] = b_val;
    }
    a_len = (size_t)n + 1;
    b_len = (size_t)n + 1;
}

/* --------------------------------------------------------------------- */

/* Return the value a(n) of the complementary sequence. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[(size_t)n];
}
