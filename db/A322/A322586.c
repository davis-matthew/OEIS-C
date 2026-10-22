/*
 * A322586  a(n) = 1 if n is a highly composite number (A002182), 0 otherwise.
 * https://oeis.org/A322586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a highly composite number (A002182), 0
 *            otherwise. The function caches divisor counts up to the maximum
 *            requested n and marks the record‑breaking counts as highly
 *            composite. Calls are O(1) after the first cache build, and each
 *            new maximum triggers a fresh recomputation up to that...
 * Range:     u8 n <= 9605, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build system */
 /* idx_t is typedef uint64_t (or similar); seq_t is an unsigned integer type.
    Their definitions are provided elsewhere, so we simply use them here. */

/* Static cache state */
static idx_t          max_n_cached = 0;   /* largest n for which arrays are filled */
static idx_t *        divs         = NULL;/* divisor counts 1..max_n_cached */
static unsigned char *hcn          = NULL;/* 1 iff index is a highly composite number */

/* Rebuilds the cache to cover at least 'need' indices.  All arrays are
   recomputed from scratch for simplicity and safety. */
static void
ensure_cache(idx_t need)
{
    if (need <= max_n_cached)
        return; /* cache already sufficient */

    /* Allocate new arrays of size need+1 (index 0 unused). */
    idx_t new_size = need + 1;
    divs = (idx_t *)calloc((size_t)new_size, sizeof(idx_t));
    hcn = (unsigned char *)calloc((size_t)new_size, sizeof(unsigned char));
    if (!divs || !hcn) {
        /* In case of allocation failure we abort – the calling context
           should handle the crash as an unrecoverable error. */
        abort();
    }

    /* Compute divisor counts for all numbers up to 'need'. */
    for (idx_t i = 1; i <= need; ++i)
        for (idx_t j = i; j <= need; j += i)
            ++divs[j];

    /* Determine highly composite numbers: record breakers of divisor count. */
    idx_t best = 0;
    for (idx_t i = 1; i <= need; ++i) {
        if (divs[i] > best) {
            best = divs[i];
            hcn[i] = 1;
        }
    }

    max_n_cached = need;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1 */

    ensure_cache(n);
    return hcn[n] ? (seq_t)1 : (seq_t)0;
}
