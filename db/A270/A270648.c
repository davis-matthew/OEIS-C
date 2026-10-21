/*
 * A270648  The sequence a of 1's and 2's starting with (2,2,2,2) such that
 *          a(n) is the length of the (n+1)st run of a.
 * https://oeis.org/A270648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(1)=2, a(2)=2, a(3)=2, a(4)=2;
 * for n ≥ 5, a(n) is the length of the (n+1)st run of the sequence.
 * The implementation constructs the sequence iteratively, storing
 * previously computed values so that any n ≥ 1 can be answered.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n<1 */

    static seq_t *cache      = NULL;   /* array of already computed terms (0‑based) */
    static size_t cap        = 0;      /* allocated capacity */
    static size_t len        = 0;      /* number of stored terms */
    static seq_t cur          = 2;     /* current run value (1 or 2) */
    static seq_t remaining    = 4;     /* remaining entries in current run */
    static idx_t  read_idx    = 0;     /* index into cache from which to fetch next run length */
    static int    init_done   = 0;     /* initialisation flag */

    /* Initialise static storage on first call. */
    if (!init_done) {
        cap = 32;                      /* initial capacity – will grow if needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) /* handle allocation failure – aborting is safest for OEIS builds */
            return (seq_t)0;
        init_done = 1;
    }

    /* We need to generate terms up to index n-1 (0‑based). */
    size_t target = (size_t)n;

    while (len < target) {
        /* Grow the array if required. */
        if (len == cap) {
            size_t new_cap = cap << 1; /* double the capacity */
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) /* allocation failed – return 0 as a safe fallback */
                return (seq_t)0;
            cache = new_cache;
            cap = new_cap;
        }

        /* Store the current term. */
        cache[len++] = cur;

        /* Advance within the current run. */
        if (--remaining == 0) {
            /* Run finished: switch value. */
            cur = (cur == 1) ? 2 : 1;
            /* Length of the next run comes from the already produced sequence. */
            remaining = cache[read_idx++];
        }
    }

    return cache[n - 1];
}
