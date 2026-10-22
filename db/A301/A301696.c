/*
 * A301696  Partial sums of A219529.
 * https://oeis.org/A301696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A301696 – partial sums of A219529
    Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5),
           a(0)=1, a(1)=6, a(2)=17, a(3)=33, a(4)=54
    The function is memoised so it works for any order of calls.
    It is written to compile in both C11 and C++17.                                   */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Initial values for n = 0..4.  They are used in both C and C++. */
static const seq_t init_vals[5] = { (seq_t)1, (seq_t)6, (seq_t)17,
                                    (seq_t)33, (seq_t)54 };

/* --------------  C build  ----------------------------------- */
#ifndef __cplusplus
static seq_t *c_cache = NULL;
static idx_t c_cache_size = 0;

static void ensure_c_cache(idx_t n) {
    if (n < c_cache_size) return;            /* already computed */

    /* Allocate or grow the array. */
    seq_t *new_mem = (seq_t *)realloc(c_cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
    if (!new_mem) abort();                    /* allocation failure */
    c_cache = new_mem;

    /* Compute missing terms. */
    for (idx_t i = c_cache_size; i <= n; ++i) {
        if (i < 5) {
            c_cache[i] = init_vals[(size_t)i];
        } else {
            c_cache[i] = (seq_t)2 * c_cache[i - 1]
                         - c_cache[i - 2]
                         + c_cache[i - 3]
                         - (seq_t)2 * c_cache[i - 4]
                         + c_cache[i - 5];
        }
    }
    c_cache_size = n + 1;
}
#endif
/* --------------  C++ build  --------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> cpp_cache;

static void ensure_cpp_cache(idx_t n) {
    if (cpp_cache.size() > n) return;        /* already computed */

    const size_t N = (size_t)n;
    const size_t old_size = cpp_cache.size();       /* before resize */
    cpp_cache.resize(N + 1);                    /* ensure size */

    /* initialise missing entries <5 (if any) */
    for (size_t i = old_size; i < 5 && i < cpp_cache.size(); ++i) {
        cpp_cache[i] = init_vals[i];
    }

    /* compute new terms */
    for (size_t i = old_size; i <= N; ++i) {
        if (i < 5) continue;                  /* already set */
        cpp_cache[i] = (seq_t)2 * cpp_cache[i - 1]
                        - cpp_cache[i - 2]
                        + cpp_cache[i - 3]
                        - (seq_t)2 * cpp_cache[i - 4]
                        + cpp_cache[i - 5];
    }
}
#endif
/* --------------  public function --------------------------------- */
seq_t a(idx_t n) {
    if (n < 5) return init_vals[(size_t)n];
#ifdef __cplusplus
    ensure_cpp_cache(n);
    return cpp_cache[(size_t)n];
#else
    ensure_c_cache(n);
    return c_cache[n];
#endif
}
