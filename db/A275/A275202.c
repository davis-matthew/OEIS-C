/*
 * A275202  Subword complexity (number of distinct blocks of length n) of the
 *          period doubling sequence A096268.
 * https://oeis.org/A275202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005942(n+1)/2. Recurrence (OEIS): a(1)=2, a(2)=3, a(2k)
 *            = 2 * a(k), a(2k-1) = a(k) + a(k-1) (k ≥ 2)
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use the build‑provided integer types */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ---------- C mode cache ------------------------------------------ */
#ifndef __cplusplus
static seq_t *a_cache = NULL;   /* array of cached values */
static size_t a_cache_cap = 0;  /* current capacity */

static void a_ensure_cache(idx_t n) {
    size_t required = (size_t)(n + 1);
    if (required <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < required) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    /* initialise new entries to 0 (unchanged values are already stored) */
    for (size_t i = a_cache_cap; i < new_cap; ++i) tmp[i] = 0;
    a_cache = tmp;
    a_cache_cap = new_cap;
}
#endif
/* ---------- C++ mode cache ---------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> a_cache_vec;   /* vector of cached values */
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n ≤ 0, return 0 */

#ifdef __cplusplus
    if (a_cache_vec.size() <= n) a_cache_vec.resize(n + 1, (seq_t)0);
    if (a_cache_vec[n] != 0) return a_cache_vec[n];

    seq_t result;
    if (n == 1)          result = 2;
    else if (n == 2)     result = 3;
    else if ((n & 1ULL) == 0)            /* n even: n = 2k */
        result = a(n >> 1) * 2;
    else                                     /* n odd: n = 2k-1 */
        result = a((n + 1) >> 1) + a(((n + 1) >> 1) - 1);

    a_cache_vec[n] = result;
    return result;
#else
    a_ensure_cache(n);
    if (a_cache[n] != 0) return a_cache[n];

    seq_t result;
    if (n == 1)          result = 2;
    else if (n == 2)     result = 3;
    else if ((n & 1ULL) == 0)            /* n even: n = 2k */
        result = a(n >> 1) * 2;
    else                                     /* n odd: n = 2k-1 */
        result = a((n + 1) >> 1) + a(((n + 1) >> 1) - 1);

    a_cache[n] = result;
    return result;
#endif
}
