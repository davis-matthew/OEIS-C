/*
 * A298001  Solution of the complementary equation a(n) = a(1)*b(n) -
 *          a(0)*b(n-1) + 3*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          b(2) = 5, and (b(n)) is the increasing sequence of positive
 *          integers not in (a(n)). See Comments.
 * https://oeis.org/A298001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequences a()/b() generated iteratively from the recurrence
   a(n) = 2*b(n) – 1*b(n-1) + 3*n with a(0)=1, a(1)=2, b(0)=3, b(1)=4, b(2)=5.
   The function maintains static caches for a, b and a boolean array of used
   integers to assign each positive integer to exactly one sequence. */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of helper functions. */
static void init(void);
static void ensure_a(size_t n);
static void ensure_b(size_t n);
static void ensure_used(size_t n);
static seq_t next_unused_value(void);

/* Static caches and state. */
static seq_t *a_cache = NULL;        /* cache for a(n) */
static size_t a_len   = 0;            /* number of a values stored */
static size_t a_cap   = 0;            /* capacity of a_cache */

static seq_t *b_cache = NULL;        /* cache for b(n) */
static size_t b_len   = 0;            /* number of b values stored */
static size_t b_cap   = 0;            /* capacity of b_cache */

static unsigned char *used = NULL;    /* bitmap of integers used by a or b */
static size_t used_cap = 0;           /* capacity of used array */

static seq_t max_used = 0;            /* maximum integer value seen */
static seq_t next_candidate = 6;      /* next integer to test for b(n) */
static bool initialized = false;

/* Ensure a_cache has at least (n+1) slots. */
static void ensure_a(size_t n) {
    if (n < a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure b_cache has at least (n+1) slots. */
static void ensure_b(size_t n) {
    if (n < b_cap) return;
    size_t new_cap = b_cap ? b_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

/* Ensure used array can address index n. */
static void ensure_used(size_t n) {
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= n) new_cap *= 2;
    unsigned char *new_used = (unsigned char *)realloc(used, new_cap);
    for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = 0;
    used = new_used;
    used_cap = new_cap;
}

/* Find the next positive integer not yet used, starting from next_candidate. */
static seq_t next_unused_value(void) {
    seq_t val = next_candidate;
    for (;;) {
        size_t idx = (size_t)val;
        ensure_used(idx + 1);
        if (!used[idx]) break;
        ++val;
    }
    next_candidate = val + 1;
    return val;
}

/* One-time initialization of caches and base terms. */
static void init(void) {
    if (initialized) return;
    initialized = true;

    /* allocate initial capacities */
    a_cap = 16; a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    b_cap = 16; b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));

    /* base values */
    a_cache[0] = 1; a_cache[1] = 2; a_len = 2;
    b_cache[0] = 3; b_cache[1] = 4; b_cache[2] = 5; b_len = 3;

    /* used array */
    used_cap = 64; used = (unsigned char *)malloc(used_cap);
    for (size_t i = 0; i < used_cap; ++i) used[i] = 0;
    /* mark base integers */
    used[1] = used[2] = used[3] = used[4] = used[5] = 1;
    max_used = 5;
    next_candidate = 6;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n) {
    init();

    /* If already computed, return it. */
    if ((idx_t)n < (idx_t)a_len) return a_cache[n];

    /* Grow caches to at least up to n. */
    for (idx_t i = (idx_t)a_len; i <= n; ++i) {
        /* Ensure b[i] exists. */
        if ((idx_t)i >= (idx_t)b_len) {
            seq_t val = next_unused_value();
            ensure_b((size_t)i);
            b_cache[i] = val;
            /* Mark used */
            size_t idx = (size_t)val;
            ensure_used(idx + 1);
            used[idx] = 1;
            if (val > max_used) max_used = val;
            ++b_len;
        }

        /* Compute a[i] for i >= 2. */
        if (i >= 2) {
            seq_t an = a_cache[1] * b_cache[i] - a_cache[0] * b_cache[i-1]
                       + (seq_t)3 * (seq_t)i;
            ensure_a((size_t)i);
            a_cache[i] = an;
            /* Mark used */
            size_t idx = (size_t)an;
            ensure_used(idx + 1);
            used[idx] = 1;
            if (an > max_used) max_used = an;
            ++a_len;
        }
    }
    return a_cache[n];
}
