/*
 * A246607  Expansion of e.g.f. exp(x - x^3).
 * https://oeis.org/A246607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) – 3*(n-1)*(n-2)*a(n-3); a(0)=1, a(1)=1, a(2)=1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t next_compute = 3;   /* next index that needs computation */

    /* Resize cache if necessary */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Initialise base cases (they are constant, so harmless to reset) */
    if (cap > 0)  cache[0] = 1;
    if (cap > 1)  cache[1] = 1;
    if (cap > 2)  cache[2] = 1;

    if (n < 3)
        return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t factor = (seq_t)3 * ((seq_t)(i - 1) * (seq_t)(i - 2));
        cache[i] = cache[i - 1] - factor * cache[i - 3];
    }
    next_compute = n + 1;
    return cache[n];
}
