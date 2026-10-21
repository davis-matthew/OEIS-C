/*
 * A264889  Partial sums of hyperfactorials (A002109).
 * https://oeis.org/A264889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} product_{i=1..k} i^i
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264889 – partial sums of hyperfactorials (A002109) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/big_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;             /* cache of a(k) values */
static size_t capacity = 0;             /* allocated size of cache   */
static idx_t current_max = (idx_t)-1;    /* highest n that has been computed   */
static seq_t H_cur = (seq_t)1;           /* current hyperfactorial H(n) */

/* initialize the cache for n = 0 */
static void init_cache(void) {
    if (!cache) {
        capacity = 1;
        cache = seq_alloc(capacity);
        cache[0] = (seq_t)1;            /* a(0) = 1    */
        current_max = 0;
        H_cur = (seq_t)1;              /* H(0) = 1    */
    }
}

/* grow the cache so that index `needed-1` is valid */
static void grow_to(size_t needed) {
    if (needed <= capacity) return;
    size_t new_cap = capacity ? capacity : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, capacity, new_cap);
    if (!tmp) abort();                 /* out‑of‑memory fallback */
    cache = tmp;
    capacity = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    init_cache();

    if (n <= current_max) {            /* already computed */
        return cache[n];
    }

    grow_to((size_t)n + 1);             /* ensure enough space */

    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t powi = big_pow((seq_t)i, (seq_t)i);  /* i^i */
        H_cur = H_cur * powi;                         /* H(i)  */
        cache[i] = cache[i - 1] + H_cur;              /* a(i)  */
    }

    current_max = n;
    return cache[n];
}
