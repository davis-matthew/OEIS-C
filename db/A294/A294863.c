/*
 * A294863  Solution of the complementary equation a(n) = a(n-2) + b(n-2) + 3,
 *          where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A294863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + b(n-2) + 3 for n ≥ 2, with a(0)=1, a(1)=2,
 *            b(0)=3 and a(n),b(n) increasing complementary sequences
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static helper functions for the used bitset */
static unsigned long long *used_bits = NULL;   /* bitset words */
static idx_t used_words = 0;                   /* number of words allocated */

static inline bool used_val(idx_t v) {
    return (used_bits[v >> 6] >> (v & 63)) & 1ULL;
}
static void ensure_used_words(idx_t v) {
    /* ensure we can address value v */
    if (v < used_words * 64) return;
    idx_t new_words = used_words ? used_words * 2 : 8;
    while (new_words * 64 <= v) new_words *= 2;
    used_bits = (unsigned long long *)realloc(used_bits, new_words * sizeof(unsigned long long));
    /* zero out new words */
    memset(used_bits + used_words, 0, (new_words - used_words) * sizeof(unsigned long long));
    used_words = new_words;
}
static inline void mark_used(idx_t v) {
    ensure_used_words(v);
    used_bits[v >> 6] |= 1ULL << (v & 63);
}

/* dynamic arrays for a and b */
static seq_t *a_vals = NULL, *b_vals = NULL;
static idx_t a_alloc = 0, b_alloc = 0;     /* allocated sizes */
static idx_t a_used = 0, b_used = 0;       /* number of stored terms */

static void ensure_a_capacity(idx_t need) {
    if (need <= a_alloc) return;
    idx_t cap = a_alloc ? a_alloc * 2 : 4;
    while (cap < need) cap *= 2;
    a_vals = (seq_t *)realloc(a_vals, cap * sizeof(seq_t));
    a_alloc = cap;
}
static void ensure_b_capacity(idx_t need) {
    if (need <= b_alloc) return;
    idx_t cap = b_alloc ? b_alloc * 2 : 4;
    while (cap < need) cap *= 2;
    b_vals = (seq_t *)realloc(b_vals, cap * sizeof(seq_t));
    b_alloc = cap;
}

/* initialization of the static state */
static void init_state(void) {
    /* allocate arrays */
    a_alloc = 4; a_vals = (seq_t *)malloc(a_alloc * sizeof(seq_t));
    b_alloc = 4; b_vals = (seq_t *)malloc(b_alloc * sizeof(seq_t));
    a_used = b_used = 0;

    /* initial terms */
    a_vals[0] = (seq_t)1; a_vals[1] = (seq_t)2; a_used = 2;
    b_vals[0] = (seq_t)3; b_used = 1;

    /* used bitset */
    ensure_used_words(3);                /* enough for 0..3 */
    mark_used((idx_t)1);
    mark_used((idx_t)2);
    mark_used((idx_t)3);
}

/* global helper for the next unused candidate */
static idx_t next_candidate = 1;

/* main function */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_state();
        initialized = true;
    }

    if (n < a_used)
        return a_vals[n];

    /* compute terms up to n */
    for (idx_t i = a_used; i <= n; ++i) {
        if (i < 2) continue;          /* already initialized */

        /* ensure b[i-2] is available */
        while (b_used <= i-2) {
            /* find next unused integer */
            while (used_val(next_candidate))
                ++next_candidate;
            seq_t val = (seq_t)next_candidate;
            mark_used((idx_t)val);
            ensure_b_capacity(b_used + 1);
            b_vals[b_used++] = val;
            ++next_candidate;          /* start search from next integer */
        }

        /* compute a[i] = a[i-2] + b[i-2] + 3 */
        seq_t a_val = a_vals[i-2] + b_vals[i-2] + (seq_t)3;
        ensure_a_capacity(i + 1);
        a_vals[i] = a_val;
        mark_used((idx_t)a_val);
        ++a_used;
    }

    return a_vals[n];
}
