/*
 * A273697  Number of 10-ary heaps on n elements.
 * https://oeis.org/A273697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1 and for n≥2 a(n) = (n-1)! / (∏ s_i!) · ∏ a(s_i)
 *            where {s_i} are the sizes of the 10 child subtrees of the root
 *            in the complete 10‑ary heap of size n. The recurrence is
 *            implemented via a multinomial coefficient built multiplicatively
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   _compute_child_sizes, _subtree_size, binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* utilities provided by the test harness */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/binom.h"     /* seq_t binom(idx_t a, idx_t b);   */
#include "../utils/_compute_child_sizes.h"
#include "../utils/_subtree_size.h"

/* --------------------------------------------------------------------- */
/*  Sub‑tree size cache helper                                            */

/* --------------------------------------------------------------------- */
/*  Memoised recurrence for a(n)                                         */
/* --------------------------------------------------------------------- */
static seq_t *cache          = NULL;   /* computed values           */
static bool   *computed      = NULL;   /* flags: value already stored */
static idx_t   cache_cap     = 0;     /* allocated size            */

static void _ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = n + 1;

    seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    bool   *new_comp = (bool   *)malloc((size_t)new_cap * sizeof(bool));

    if (cache) {
        memcpy(new_cache, cache, (size_t)cache_cap * sizeof(seq_t));
        memcpy(new_comp, computed, (size_t)cache_cap * sizeof(bool));
        free(cache);
        free(computed);
    }

    /* Initialise freshly allocated entries */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        new_comp[i] = false;            /* not yet computed */
    }

    cache    = new_cache;
    computed = new_comp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Sequence function                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Ensure cache space is available */
    _ensure_cache(n);
    if (computed[n]) {
        return cache[n];
    }

    /* Compute the sizes of the 10 child subtrees */
    idx_t child_sizes[10];
    _compute_child_sizes(n, child_sizes);

    /* Multinomial coefficient: (n-1)! / (∏ s_i!)   */
    seq_t mult = (seq_t)1;
    idx_t remaining = n - 1;
    for (int i = 0; i < 10; ++i) {
        idx_t s = child_sizes[i];
        if (s == 0) continue;
        mult = seq_mul(mult, binom(remaining, s));
        remaining -= s;
    }

    /* Multiply by a(s_i) for all children */
    seq_t result = mult;
    for (int i = 0; i < 10; ++i) {
        idx_t s = child_sizes[i];
        if (s == 0) continue;
        result = seq_mul(result, a(s));
    }

    /* Store in cache and return */
    cache[n] = result;
    computed[n] = true;
    return result;
}
