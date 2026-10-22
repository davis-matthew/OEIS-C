/*
 * A377196  Expansion of 1/(1 - 4*x*(1+x)^2)^(7/2).
 * https://oeis.org/A377196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377196
   Expansion of 1/(1 - 4*x*(1+x)^2)^(7/2)
   Recurrence: a(0)=1, a(1)=14, a(2)=154;
   a(n) = (2*(2*n+5)*a(n-1) + 8*(n+5)*a(n-2) + 2*(2*n+15)*a(n-3))/n   (n>=3) */
#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

/* Cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;          /* allocated size */
static idx_t last_computed = 0;       /* highest index already computed */

/* Helper: multiply seq_t by unsigned int
   In C++ mode we use specialisation for unsigned long long to avoid overflow
   by using __int128 intermediate. In C mode we rely on __int128 directly. */
#if defined(__cplusplus)
/* C++ version */
static inline seq_t mul_by_uint(seq_t a, unsigned int m) {
    if constexpr (std::is_same<seq_t, unsigned long long>::value) {
        return (seq_t)((__int128)a * m);
    } else {
        return a * m;
    }
}
#else
/* C version */
static inline seq_t mul_by_uint(seq_t a, unsigned int m) {
    return (seq_t)((__int128)a * m);
}
#endif

seq_t a(idx_t n) {
    /* Handle the initial values directly. */
    if (n == 0) return 1;
    if (n == 1) return 14;
    if (n == 2) return 154;

    /* Allocate cache if needed or extend it. */
    if (cache == NULL) {
        cache_size = (n >= 3 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failed */
        cache[0] = 1;
        cache[1] = 14;
        cache[2] = 154;
        last_computed = (cache_size > 2 ? 2 : cache_size - 1);
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0;    /* allocation failed */
        cache = new_cache;
        cache_size = new_size;
    }

    /* If the requested term is already computed, return it. */
    if (n <= last_computed) return cache[n];

    /* Compute terms sequentially up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
#if defined(__cplusplus)
        /* In C++ mode, use the helper that may use __int128 for 64‑bit seq_t. */
        seq_t t1 = mul_by_uint(cache[i - 1], 4 * (unsigned long long)i + 10);
        seq_t t2 = mul_by_uint(cache[i - 2], 8 * (unsigned long long)i + 40);
        seq_t t3 = mul_by_uint(cache[i - 3], 4 * (unsigned long long)i + 30);
        cache[i] = (t1 + t2 + t3) / (seq_t)i;
#else
        /* In C mode, perform the computation with __int128 intermediate. */
        __int128 s1 = (__int128)cache[i - 1] * (4 * (int)i + 10);
        __int128 s2 = (__int128)cache[i - 2] * (8 * (int)i + 40);
        __int128 s3 = (__int128)cache[i - 3] * (4 * (int)i + 30);
        __int128 sum = s1 + s2 + s3;
        cache[i] = (seq_t)(sum / (seq_t)i);
#endif
    }
    last_computed = n;
    return cache[n];
}
