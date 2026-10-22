/*
 * A366314  a(n) = a(n-1) + 3*a(n-2) + 9*a(n-3) with a(0)=0, a(1)=1, a(2)=4.
 * https://oeis.org/A366314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 9*a(n-3), with a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache  = NULL;   /* table of computed values */
static idx_t  max_safe = 0;    /* highest index stored */

static void ensure_cache(idx_t n)
{
    if (n <= max_safe)
        return;     /* already computed */

    /* size needed: indices 0 … n inclusive */
    idx_t new_size = n + 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Memory allocation failed – terminate the program. */
        abort();
    }
    cache = tmp;

    /* On first allocation initialise the first three terms. */
    if (max_safe == 0) {
        cache[0] = (seq_t)0;
        if (new_size > 1)
            cache[1] = (seq_t)1;
        if (new_size > 2)
            cache[2] = (seq_t)4;
        max_safe = 2;
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = max_safe + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        cache[i] = v1 + ((seq_t)3 * v2) + ((seq_t)9 * v3);
    }
    max_safe = n;
}

seq_t a(idx_t n)
{
    /* Handle the trivial small indices directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Ensure all terms up to n are computed. */
    ensure_cache(n);
    return cache[n];
}
