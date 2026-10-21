/*
 * A293121  Expansion of e.g.f. exp(x^3/(1+x)).
 * https://oeis.org/A293121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of xⁿ in exp(x³/(1+x)) multiplied by n!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=0, a(2)=0
   For k≥3:
     a(k) = Σ_{i=0}^{k-3} ((k-1)!/i!) * a(i) * (k-i) * (-1)^{k-i-3}
*/

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static idx_t computed_to = 0;

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static std::vector<seq_t> fact_vec;
static void ensure_cache(size_t n)
{
    if (cache_vec.size() <= n) {
        cache_vec.resize(n + 1);
        fact_vec.resize(n + 1);
    }
}
#else
static seq_t *cache_ptr = NULL;
static seq_t *fact_ptr  = NULL;
static size_t cache_cap = 0;
static void ensure_cache(size_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= n) new_cap = n + 1;
        cache_ptr = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
        fact_ptr  = (seq_t *)realloc(fact_ptr , new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}
#endif

seq_t a(idx_t n)
{
    if ((idx_t)n < computed_to) {
#ifdef __cplusplus
        return cache_vec[n];
#else
        return cache_ptr[n];
#endif
    }

#ifdef __cplusplus
    ensure_cache((size_t)n);
    if (cache_vec[0] == 0 && fact_vec[0] == 0) {
        cache_vec[0] = 1;
        fact_vec[0]  = 1;
    }
#else
    ensure_cache((size_t)n);
    if (cache_ptr[0] == 0 && fact_ptr[0] == 0) {
        cache_ptr[0] = 1;
        fact_ptr[0]  = 1;
    }
#endif

    for (idx_t k = computed_to; k <= n; ++k) {
        if (k == 0) {
            /* already initialised */
            continue;
        }
        if (k == 1 || k == 2) {
#ifdef __cplusplus
            cache_vec[k] = 0;
            fact_vec[k]  = fact_vec[k - 1] * (seq_t)k;
#else
            cache_ptr[k] = 0;
            fact_ptr[k]  = fact_ptr[k - 1] * (seq_t)k;
#endif
            continue;
        }

        seq_t sum;
        seq_t fact_kminus1;
#ifdef __cplusplus
        fact_kminus1 = fact_vec[k - 1];
        sum = 0;
        for (idx_t i = 0; i <= k - 3; ++i) {
            seq_t term = fact_kminus1 / fact_vec[i];
            term *= cache_vec[i];
            term *= (seq_t)(k - i);
            if (((k - i - 3) & 1)) term = -term;
            sum += term;
        }
        cache_vec[k] = sum;
        fact_vec[k]  = fact_kminus1 * (seq_t)k;
#else
        fact_kminus1 = fact_ptr[k - 1];
        sum = 0;
        for (idx_t i = 0; i <= k - 3; ++i) {
            seq_t term = fact_kminus1 / fact_ptr[i];
            term *= cache_ptr[i];
            term *= (seq_t)(k - i);
            if (((k - i - 3) & 1)) term = -term;
            sum += term;
        }
        cache_ptr[k] = sum;
        fact_ptr[k]  = fact_kminus1 * (seq_t)k;
#endif
    }

    computed_to = n + 1;

#ifdef __cplusplus
    return cache_vec[n];
#else
    return cache_ptr[n];
#endif
}
