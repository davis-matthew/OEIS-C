/*
 * A283895  Relative of Hofstadter Q-sequence: a(1) = 41, a(2) = 2; thereafter
 *          a(n) = a(n-a(n-1)) + a(n-a(n-2)).
 * https://oeis.org/A283895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 41, a(2) = 2; thereafter a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)), with a(n) = 0 for n <= 0
 * Range:     u8 n <= 98, u16 n <= 9504, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* 0‑based index 0 is unused; we keep index 1..N to store a(n).
   When a(n) == 0 we treat the entry as "not yet computed". */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can address indices up to 'idx'.  Newly allocated
   slots are zero‑initialized. */
static void ensure_capacity(idx_t idx)
{
    size_t needed = (size_t)idx + 1;    /* indices 0..idx inclusive */
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    /* Reallocate and zero‑initialize the new part. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;
}

/* Recursive, memoized evaluation of a(n).  The function assumes
   that 'n' fits in idx_t.  For idx_t < 1 the result is 0 by
   convention, so no cache entry is stored for such indices. */
static seq_t rec(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 by definition */

    /* Store base values explicitly. */
    if (n == 1) return 41;
    if (n == 2) return 2;

    /* Ensure we have space to memoize the result. */
    ensure_capacity(n);
    seq_t *slot = cache + n;
    if (*slot != 0) return *slot;  /* already computed */

    /* Compute subindices safely: n - a(n-1) and n - a(n-2). */
    seq_t a_prev  = rec(n - 1);
    seq_t a_prevp = rec(n - 2);

    idx_t k1 = 0, k2 = 0;
    if ((seq_t)n > a_prev)   k1 = n - (idx_t)a_prev;
    if ((seq_t)n > a_prevp)  k2 = n - (idx_t)a_prevp;

    /* Recursively compute the two terms that are summed. */
    seq_t v1 = rec(k1);
    seq_t v2 = rec(k2);

    *slot = v1 + v2;  /* memoize and return */
    return *slot;
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    return rec(n);
}
