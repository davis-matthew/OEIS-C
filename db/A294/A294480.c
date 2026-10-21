/*
 * A294480  Solution of the complementary equation a(n) = a(n-2) + b(n-1) +
 *          2n, where a(0) = 1, a(1) = 3, b(0) = 2.
 * https://oeis.org/A294480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + b(n-1) + 2n a(0) = 1, a(1) = 3, b(0) = 2 The
 *            complementary sequence b(n) consists of the n-th positive
 *            integer not occurring in a(). The function memoises
 *            already‑computed values so it works in any call order. The
 *            implementation uses a bitset to track used numbers and grows
 *            linear...
 * Range:     u8 n <= 16, u16 n <= 290, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* forward declaration of cache helpers */
static void init_cache(void);
static void ensure_up_to(idx_t n);
static uint64_t next_unused(void);
static void set_used(uint64_t v);
static int test_used(uint64_t v);

/* global cache state */
static seq_t *a_cache = NULL;          /* a values   */
static size_t a_len = 0, a_cap = 0;    /* cache length and capacity   */
static seq_t *b_cache = NULL;          /* b values   */
static size_t b_len = 0, b_cap = 0;    /* cache length and capacity   */
static uint64_t *used_bits = NULL;     /* bitset of used numbers      */
static size_t used_cap = 0;            /* number of 64‑bit words      */
static uint64_t next_candidate = 1;    /* next number to examine for b */
static int initialized = 0;            /* flag whether cache has been built */

/* set the bit corresponding to number v */
static void set_used(uint64_t v)
{
    size_t idx = v >> 6; /* word index */
    if (idx >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 1;
        while (idx >= new_cap) new_cap <<= 1;
        uint64_t *p = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!p) abort();
        /* clear newly added words */
        memset(p + used_cap, 0, (new_cap - used_cap) * sizeof(uint64_t));
        used_bits = p;
        used_cap = new_cap;
    }
    used_bits[idx] |= 1ULL << (v & 63);
}

/* check if number v has already been used */
static int test_used(uint64_t v)
{
    size_t idx = v >> 6;
    if (idx >= used_cap) return 0;
    return (used_bits[idx] >> (v & 63)) & 1ULL;
}

/* get the next unused integer (for b) */
static uint64_t next_unused(void)
{
    uint64_t cand = next_candidate;
    do {
        cand++;
    } while (test_used(cand));
    next_candidate = cand;
    return cand;
}

/* initialise caches and the first few elements */
static void init_cache(void)
{
    /* a(0)=1, a(1)=3 */
    a_cap = 4;
    a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!a_cache) abort();
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)3;
    a_len = 2;

    /* b(0)=2 */
    b_cap = 2;
    b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
    if (!b_cache) abort();
    b_cache[0] = (seq_t)2;
    b_len = 1;

    /* used bitset start */
    used_cap = 1;
    used_bits = (uint64_t *)malloc(used_cap * sizeof(uint64_t));
    if (!used_bits) abort();
    used_bits[0] = 0;
    set_used(1);   /* a(0) */
    set_used(2);   /* b(0) */
    set_used(3);   /* a(1) */
    next_candidate = 1; /* next search starts after 1 */
    initialized = 1;
}

/* ensure that a_cache contains all a(0)…a(n) and b_cache contains b(0)…b(n‑1) */
static void ensure_up_to(idx_t n)
{
    while (a_len <= n) {
        /* make sure we have b[n‑1] */
        while (b_len <= a_len - 1) {
            uint64_t cand = next_unused();
            set_used(cand);
            if (b_len >= b_cap) {
                b_cap <<= 1;
                b_cache = (seq_t *)realloc(b_cache, b_cap * sizeof(seq_t));
                if (!b_cache) abort();
            }
            b_cache[b_len++] = (seq_t)cand;
        }
        /* compute a[ a_len ] */
        idx_t i = (idx_t)a_len;
        seq_t val = a_cache[i - 2] + b_cache[i - 1] + (seq_t)i * 2UL;
        if (a_len >= a_cap) {
            a_cap <<= 1;
            a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
            if (!a_cache) abort();
        }
        a_cache[a_len++] = val;
        set_used((uint64_t)val);
    }
}

/* the required function */
seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    ensure_up_to(n);
    return a_cache[n];
}
