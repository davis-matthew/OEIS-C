/*
 * A398808  a(n) = 27^n * [z^n] 2F1(1/3, 1/3; 1; z)^2.
 * https://oeis.org/A398808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27^n * [z^n] 2F1(1/3, 1/3; 1; z)^2 * Recurrence: n^3*a(n)
 *            = 6*(3*n-2)*(3*n^2-4*n+2)*a(n-1) * - 27*(3*n-4)^3*a(n-2) *
 *            Initial values: a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
using big_t = typename std::conditional<std::is_integral<seq_t>::value,
                                       unsigned __int128, seq_t>::type;
#else
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 big_t;
#else
typedef unsigned long long big_t;
#endif
#endif

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t n) {
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache_len = 2;
    }

    if ((idx_t)cache_len > n) return cache[(size_t)n];

    ensure_capacity((size_t)n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)6;
        } else {
            big_t ib = (big_t)i;
            big_t term1 = (big_t)6 *
                          ((big_t)3 * ib - (big_t)2) *
                          ((big_t)3 * ib * ib - (big_t)4 * ib + (big_t)2);
            big_t cube_factor = (big_t)3 * ib - (big_t)4;
            big_t term2 = (big_t)27 *
                          cube_factor * cube_factor * cube_factor;
            big_t rhs = term1 * (big_t)cache[i-1] -
                        term2 * (big_t)cache[i-2];
            big_t denom = ib * ib * ib;
            big_t an_big = rhs / denom;
            cache[i] = (seq_t)an_big;
        }
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
