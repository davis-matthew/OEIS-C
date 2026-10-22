/*
 * A354960  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number not occurring earlier that is a multiple of the number of
 *          proper divisors of a(n-1).
 * https://oeis.org/A354960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 6814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/floor_sqrt.h"      /* seq_t floor_sqrt(seq_t x) */
#include "../utils/alloc_seq_array.h"/* seq_t *alloc_seq_array(size_t n) */

/* Sequence A354960
 *
 * a(1) = 1, a(2) = 2.
 * For n > 2, a(n) is the smallest positive integer not appearing in
 * a(1)..a(n‑1) that is a multiple of the number of proper divisors of a(n‑1).
 */

static seq_t *seq_cache   = NULL;   /* cached sequence values (0‑based) */
static size_t seq_cap     = 0;      /* current capacity of seq_cache     */
static size_t seq_len     = 0;      /* number of values stored           */

static unsigned char *seen      = NULL;  /* 0 = not seen, 1 = seen   */
static size_t seen_cap          = 0;      /* capacity of seen array      */

static bool init_done = false;            /* one‑time initialization flag */

/* Ensure 'seen' can hold indices up to 'need' (inclusive) */
static void ensure_seen_capacity(idx_t need)
{
    if ((idx_t)seen_cap > need) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 4;
    while ((idx_t)new_cap <= need) new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
    if (!tmp) abort();                     /* allocation failure */
    memset(tmp + seen_cap, 0, new_cap - seen_cap);
    seen   = tmp;
    seen_cap = new_cap;
}

/* Count the proper divisors of n (n ≥ 1). */
static seq_t count_proper_divisors(seq_t n)
{
    if (n <= 1) return 0;                  /* 1 has no proper divisors */
    idx_t root = (idx_t)floor_sqrt(n);
    seq_t count = 0;
    for (idx_t i = 1; i <= root; ++i) {
        if (n % (seq_t)i == 0) {
            seq_t j = n / (seq_t)i;
            if (i == j) {
                if ((seq_t)i != n) ++count;
            } else {
                if ((seq_t)i != n) ++count;
                if (j != n)     ++count;
            }
        }
    }
    return count;
}

/* Initialise the cache with the first two terms. */
static void init_cache(void)
{
    if (init_done) return;
    init_done = true;

    seq_cap = 64;                           /* initial capacity */
    seq_cache = alloc_seq_array(seq_cap);
    if (!seq_cache) abort();

    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)2;
    seq_len = 2;

    seen_cap = 8;                            /* enough for 1 and 2 */
    seen = (unsigned char *)calloc(seen_cap, 1);
    if (!seen) abort();
    seen[1] = 1;
    seen[2] = 1;
}

/* Compute the sequence up to index n (1‑based). */
static void ensure_sequence_up_to(idx_t n)
{
    if (n <= seq_len) return;
    init_cache();

    while (seq_len < n) {
        /* Grow backing array if necessary */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = alloc_seq_array(new_cap);
            if (!tmp) abort();
            memcpy(tmp, seq_cache, seq_len * sizeof(seq_t));
            free(seq_cache);
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        seq_t prev = seq_cache[seq_len - 1];
        seq_t d    = count_proper_divisors(prev);
        if (d == 0) d = (seq_t)1;  /* safety, shouldn't happen */

        seq_t cand = d;
        for (;;) {
            ensure_seen_capacity((idx_t)cand);
            if (!seen[(size_t)cand]) break;
            cand += d;
        }

        seq_cache[seq_len] = cand;
        ensure_seen_capacity((idx_t)cand);
        seen[(size_t)cand] = 1;
        ++seq_len;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_sequence_up_to(n);
    return seq_cache[n - 1];   /* 0‑based array, 1‑based index */
}
