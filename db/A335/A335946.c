/*
 * A335946  a(n) = 1 + Sum_{k=0..n-1} binomial(n,k)^2 * a(k).
 * https://oeis.org/A335946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ∑_{k=0}^{n-1} binomial(n,k)^2 · a(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>
#include "../utils/binomial.h"

#ifndef __cplusplus
/* --------- C implementation ------------------------------------------------- */
static seq_t *cache_c = NULL;
static size_t cache_cap_c = 0;

static void ensure_cache_c(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap_c)
        return;
    /* grow capacity: at least n+1, at least 16, double if needed */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 16)
        new_cap = 16;
    if (new_cap < 2 * cache_cap_c)
        new_cap = 2 * cache_cap_c;

    seq_t *tmp = (seq_t *)realloc(cache_c, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – leave cache unchanged; caller may handle */
        return;
    }
    cache_c = tmp;

    /* compute any new terms */
    for (size_t i = cache_cap_c; i < new_cap; ++i) {
        if (i == 0) {
            cache_c[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t b = binomial(i, k);
                sum += b * b * cache_c[k];
            }
            cache_c[i] = 1 + sum;
        }
    }
    cache_cap_c = new_cap;
}
#endif

#ifdef __cplusplus
/* --------- C++ implementation ------------------------------------------------- */
#include <vector>
#include <cstddef>

static std::vector<seq_t> cache_cpp;

static void ensure_cache_cpp(idx_t n)
{
    if (cache_cpp.size() > (size_t)n)
        return;
    size_t old_size = cache_cpp.size();
    cache_cpp.resize((size_t)n + 1);
    /* ensure a(0) is set */
    if (old_size == 0) {
        cache_cpp[0] = (seq_t)1;
        old_size = 1;
    }
    /* compute any new terms */
    for (size_t i = old_size; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t b = binomial(i, k);
            sum += b * b * cache_cpp[k];
        }
        cache_cpp[i] = 1 + sum;
    }
}
#endif

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    ensure_cache_cpp(n);
    return cache_cpp[(size_t)n];
#else
    ensure_cache_c(n);
    return cache_c[(size_t)n];
#endif
}
