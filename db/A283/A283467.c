/*
 * A283467  a(n) = A005185(n+1-A005185(n)).
 * https://oeis.org/A283467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005185(n + 1 - A005185(n))
 * Range:     u8 n <= 540, u16 n <= 9763, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Hofstadter Q‑sequence A005185 is generated on demand
   and cached.  The term a(n) is then obtained directly
   from that cache.  All arithmetic conforms to the
   required types.  */
/* This implementation works for C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache for the Hofstadter Q‑sequence */
static seq_t *q_cache = NULL;   /* array of A005185 values, 1‑based */
static idx_t q_limit = 0;       /* highest index computed so far */

/* Ensure that q_cache has entries up to at least 'n'.
   Computes new terms using the Hofstadter recurrence. */
static void ensure_q_upto(idx_t n)
{
    if (n <= q_limit)
        return;                    /* already computed */

    /* Choose a new capacity – at least double the current, enough for n+. */
    idx_t new_cap = q_limit ? q_limit * 2 : 128;
    while (new_cap < n + 1)
        new_cap <<= 1;

    if (q_cache == NULL) {
        /* Allocate the array: index 0 is unused, indices 1..new_cap are valid. */
        q_cache = seq_alloc(new_cap + 1);
    } else {
        /* Reallocate to the larger size. */
        q_cache = seq_realloc(q_cache, q_limit + 1, new_cap + 1);
    }

    /* Compute any new values from (q_limit + 1) up to n. */
    for (idx_t i = q_limit + 1; i <= n + 1; ++i) {
        if (i == 1 || i == 2) {
            q_cache[i] = (seq_t)1;
        } else {
            /* Indices for the recurrence are guaranteed to be valid
               because q_cache is filled in increasing order. */
            idx_t idx1 = (idx_t)q_cache[i - 1];
            idx_t idx2 = (idx_t)q_cache[i - 2];
            seq_t  v1  = q_cache[i - idx1];
            seq_t  v2  = q_cache[i - idx2];
            q_cache[i] = v1 + v2;
        }
    }
    q_limit = n + 1;
}

/* Public function implementing the OEIS sequence A283467. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* behavior for n < 1 is undefined – return 0 */

    /* Make sure q_cache contains Q(n) and possibly Q(n+1). */
    ensure_q_upto((idx_t)n + 1);

    seq_t qn = q_cache[n];              /* A005185(n) */
    idx_t idx = (idx_t)qn;               /* safe: qn <= n for all n */
    idx_t target = n + 1 - idx;          /* index for A005185 in a(n) */

    /* Ensure the CACHE contains this target index as well. */
    ensure_q_upto(target);

    return q_cache[target];             /* A005185(n + 1 - A005185(n)) */
}
