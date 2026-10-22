/*
 * A318621  a(0) = a(1) = 1; for n > 1, a(n) = Sum_{k=0..n-2} a(k) OR
 *          a(n-k-2).
 * https://oeis.org/A318621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; for n > 1, a(n) = ∑_{k=0..n-2} (a(k) OR
 *            a(n-k-2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;    /* memoisation storage */
static idx_t  cap   = 0;       /* current buffer capacity */

/* Ensure that the cache can hold indices up to 'needed'.
   Newly allocated memory is zero‑initialised. */
static void ensure_cache(idx_t needed)
{
    if (needed < cap)
        return;
    size_t old_cap = cap;
    size_t new_cap = (cap == 0) ? 4 : cap * 2;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {          /* allocation failed – abort safely */
        free(cache);
        exit(EXIT_FAILURE);
    }
    /* Zero any new area */
    if (new_cap > old_cap)
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));

    cache = tmp;
    cap   = new_cap;
}

/* Return the n‑th term of A318621. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    ensure_cache(n);

    /* Initialise base cases if this is the first use of the cache. */
    if (cache[0] == 0) cache[0] = 1;
    if (cache[1] == 0) cache[1] = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != 0)  /* already computed */
            continue;

        seq_t sum = 0;
        for (idx_t k = 0; k <= i - 2; ++k)
            sum += (cache[k] | cache[i - k - 2]);

        cache[i] = sum;
    }

    return cache[n];
}
