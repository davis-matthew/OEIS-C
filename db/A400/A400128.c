/*
 * A400128  Number of recursively defined forms of weight n, where a form is
 *          either a unit, an ordered fusion of two forms, or a propagation of
 *          one form whose weight is tripled.
 * https://oeis.org/A400128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n‑1} a(k)*a(n‑k) + [n ≡ 0 (mod 3)]*a(n/3) a(1) =
 *            1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1535 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Ensure cache can hold indices up to n (capacity = max index). */
static void _ensure_cache(idx_t n, seq_t **cache, idx_t *cap)
{
    if (n < *cap) return;                 /* index n needs n < cap (was n <= cap) */

    idx_t new_cap = (*cap) ? (*cap << 1) : 4;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp = mem_new(seq_t, (size_t)new_cap);
    if (!tmp) abort();

    /* copy existing values */
    if (*cache) mem_copy(tmp, *cache, (size_t)(*cap) * sizeof(seq_t));
    mem_free(*cache);

    /* zero new entries via assignment to handle arbitrary‑precision types */
    idx_t i;
    for (i = *cap; i < new_cap; ++i) tmp[i] = (seq_t)0;

    *cache = tmp;
    *cap   = new_cap;
}

/* Public function computing a(n). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    if (n < 1) return (seq_t)0; /* undefined for n<1, but safe */

    _ensure_cache(n, &cache, &cap);

    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0) continue;        /* already computed */

        seq_t sum = (seq_t)0;
        idx_t k;

        /* convolution term: ∑ a(k)*a(i‑k), k = 1 … i‑1 */
        for (k = 1; k < i; ++k)
            sum += cache[k] * cache[i - k];

        /* propagation term for multiples of 3 */
        if ((i % 3) == 0U)
            sum += cache[i / 3];

        cache[i] = sum;
    }

    return cache[n];
}
