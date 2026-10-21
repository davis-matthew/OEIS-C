/*
 * A274270  Expansion of e.g.f. (1 + x)^5*log(1 + x).
 * https://oeis.org/A274270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   for n >= 6, a(n) = (-1)^n * 120 * (n-6)! For n = 1..5, the terms
 *            are: 1, 9, 47, 154, 274
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 16, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first five terms (special handling) */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)47;
    if (n == 4) return (seq_t)154;
    if (n == 5) return (seq_t)274;

    /* n >= 6 -------------------------------------------------------- */
    /* compute (n-6)! with memoisation */
#ifdef __cplusplus
    /* C++: use std::vector as cache */
    static std::vector<seq_t> fact_cache;  /* fact_cache[i] = i! */
    if (fact_cache.empty()) {
        fact_cache.reserve(8);      /* small initial reserve */
        fact_cache.push_back((seq_t)1); /* 0! = 1 */
    }
    idx_t k = n - 6;           /* factorial index */
    while ((idx_t)fact_cache.size() <= k) {
        /* next factorial */
        idx_t i = fact_cache.size();        /* i == current size = previous factorial index +1 */
        seq_t next = fact_cache.back() * (seq_t)i;
        fact_cache.push_back(next);
    }
    seq_t fact = fact_cache[k];
#else
    /* C: use malloc/realloc as cache */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cap = 0;              /* number of entries stored (size of array) */
    idx_t k = n - 6;
    if (k >= fact_cap) {
        idx_t new_cap = k + 1;
        seq_t *new_mem = (seq_t *)realloc(fact_cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_mem) abort();              /* handle allocation failure */
        fact_cache = new_mem;
        /* initialise from previous capacity to new size */
        if (fact_cap == 0) {
            fact_cache[0] = (seq_t)1;       /* 0! = 1 */
            fact_cap = 1;
        }
        for (idx_t i = fact_cap; i <= k; ++i) {
            seq_t next = fact_cache[i - 1] * (seq_t)i;
            fact_cache[i] = next;
        }
        fact_cap = new_cap;
    }
    seq_t fact = fact_cache[k];
#endif

    /* compute the term */
    seq_t result = fact * (seq_t)120;
    if (n & 1) { /* odd n: multiply by -1 */
        result = -result;
    }
    return result;
}
