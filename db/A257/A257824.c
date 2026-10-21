/*
 * A257824  Positive integers whose square is the sum of 73 consecutive
 *          squares.
 * https://oeis.org/A257824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4562498*a(n‑4) – a(n‑8)   (n ≥ 9)
   Offset: 1   (a(1) = 4088) */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t, etc. */
#include <stdlib.h>   /* malloc, realloc */

static const seq_t init8[8] = {
    (seq_t)4088ULL,
    (seq_t)23360ULL,
    (seq_t)1582640ULL,
    (seq_t)9047912ULL,
    (seq_t)18642443912ULL,
    (seq_t)106578370640ULL,
    (seq_t)7220791811360ULL,
    (seq_t)41281080400088ULL
};

static seq_t *cache = NULL;        /* Allocated cache for computed terms   */
static size_t cached_count = 0;    /* Number of terms currently stored     */
static size_t cached_cap   = 0;    /* Current allocation capacity          */

/* Extend the cache allocation if needed. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cached_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for the OEIS offset */

    /* Directly return the first eight terms. */
    if (n <= 8)
        return init8[(size_t)n - 1];

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        /* Allocate enough space for the requested term and a small buffer. */
        size_t init_cap = n > 16 ? n : 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL)       /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        /* Copy the hard‑coded initial eight terms. */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init8[i];
        cached_count = 8;
        cached_cap   = init_cap;
    }

    /* Extend the cache up to the requested index. */
    if (cached_count < (size_t)n) {
        ensure_capacity((size_t)n);
        /* Compute new terms using the linear recurrence. */
        for (idx_t i = (idx_t)cached_count + 1; i <= n; ++i) {
            seq_t term = (seq_t)4562498 * cache[(size_t)i - 5]  /* a(i‑4) */
                         - cache[(size_t)i - 9];               /* a(i‑8) */
            cache[(size_t)i - 1] = term;
        }
        cached_count = (size_t)n;
    }

    /* Return the required term. */
    return cache[(size_t)n - 1];
}
