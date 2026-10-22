/*
 * A400244  G.f. A(x) satisfies A(x) = A(x^3/(1 - x)^4) / (1 - x)^2, with A(0)
 *          = 1.
 * https://oeis.org/A400244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊n/3⌋} binomial(n+k+1,4k+1) * a(k)
 *            (OEIS A400244)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n, idx_t k); */

/* Persistent cache – 0‑based index */
static seq_t *cache  = NULL;
static bool   *done   = NULL;
static idx_t   cap    = 0;

/* Ensure the cache can hold indices 0..n */
static void ensure_cache(idx_t n)
{
    if (n < cap) return;

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    bool  *new_done  = (bool*)realloc(done,  new_cap * sizeof(bool));
    if (!new_cache || !new_done) abort();

    /* New entries are initially not computed */
    for (idx_t i = cap; i < new_cap; ++i) new_done[i] = false;

    cache = new_cache;
    done  = new_done;
    cap   = new_cap;

    /* a(0) = 1; guard against n==0 calls */
    if (!done[0]) { cache[0] = (seq_t)1; done[0] = true; }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    for (idx_t i = 0; i <= n; ++i) {
        if (done[i]) continue;          /* already computed */

        seq_t sum = 0;
        idx_t maxk = i / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binom(i + k + 1, 4 * k + 1);
            sum += bin * cache[k];
        }
        cache[i] = sum;
        done[i]  = true;
    }
    return cache[n];
}
