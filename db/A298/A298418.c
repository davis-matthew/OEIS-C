/*
 * A298418  Solution (c(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A298418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = solution of the system a(n) = least new b(n) = least new
 *            >= a(n) + n + 1 c(n) = a(n) + b(n) The function returns c(n)
 * Range:     u8 n <= 48, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*--- Variable-length bitset for used numbers ---------------------------------*/
static uint8_t *used_bits = NULL;      /* each byte holds 8 booleans */
static idx_t   used_cap_bytes = 0;     /* capacity in bytes */

/* ensure 'used_bits' can store bit for index 'x' */
static void ensure_used_capacity(idx_t x)
{
    if (x < used_cap_bytes * 8u) return;
    idx_t new_cap = used_cap_bytes ? used_cap_bytes * 2u : 1u;
    while (new_cap * 8u <= x) new_cap *= 2u;
    uint8_t *new_bits = (uint8_t*)realloc(used_bits, (size_t)new_cap);
    if (!new_bits) abort();                 /* out‑of‑memory */
    if (new_cap > used_cap_bytes)
        memset(new_bits + used_cap_bytes, 0,
               (size_t)(new_cap - used_cap_bytes));
    used_bits     = new_bits;
    used_cap_bytes = new_cap;
}

/* test whether integer 'x' is already used */
static inline bool used_get(idx_t x)
{
    return x < used_cap_bytes * 8u ? (used_bits[x >> 3] & (1u << (x & 7u))) : false;
}

/* mark integer 'x' as used */
static inline void used_set(idx_t x)
{
    ensure_used_capacity(x);
    used_bits[x >> 3] |= (1u << (x & 7u));
}

/*--- Memoised cache for c(n) ----------------------------------------*/
static seq_t *c_cache      = NULL;   /* array of computed c values */
static size_t c_capacity   = 0;      /* allocated size */
static idx_t  max_computed = (idx_t)-1;  /* last index computed (-1 → none yet) */

/* ensure the cache can hold at least 'size' elements */
static void ensure_cache_capacity(size_t size)
{
    if (size <= c_capacity) return;
    size_t new_cap = c_capacity ? c_capacity * 2u : 16u;
    while (new_cap < size) new_cap *= 2u;
    seq_t *new_cache = (seq_t*)realloc(c_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();             /* out‑of‑memory */
    c_cache     = new_cache;
    c_capacity  = new_cap;
}

/*--- Main function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (max_computed == (idx_t)-1) {
        ensure_cache_capacity(1);
        /* base values a0=1, b0=2, c0=3 */
        c_cache[0] = (seq_t)3;          /* c(0) */
        used_set((idx_t)1);
        used_set((idx_t)2);
        used_set((idx_t)3);
        max_computed = 0;
    }

    /* extend cache up to required index 'n' if necessary */
    if (n > max_computed) {
        static idx_t next_unused = 1;   /* cursor for next candidate for a(n) */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* find a(i): least unused integer */
            while (used_get(next_unused)) ++next_unused;
            seq_t a_i = (seq_t)next_unused;
            used_set(next_unused);

            /* find b(i): least unused integer >= a_i + i + 1 */
            idx_t cand = (idx_t)a_i + i + 1;
            while (used_get(cand)) ++cand;
            seq_t b_i = (seq_t)cand;
            used_set(cand);

            /* compute c(i) */
            seq_t c_i = a_i + b_i;
            ensure_cache_capacity((size_t)(i + 1));
            c_cache[i] = c_i;
            /* mark c(i) as used if it fits in idx_t */
            if (c_i <= (seq_t)((idx_t)-1))
                used_set((idx_t)c_i);

            /* advance cursor for next a calculation */
            ++next_unused;
        }
        max_computed = n;
    }

    return c_cache[n];
}
