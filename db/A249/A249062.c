/*
 * A249062  A double binomial sum.
 * https://oeis.org/A249062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+4)+(n+1)*a(n+3)-3*(n+3)*a(n+2)-3*(n+3)*(n+2)*a(n+1)-(n+3)*(n+2)*(n+1)*a(n)=0,
 *            with a(0)=1, a(1)=2, a(2)=5, a(3)=18
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

#ifdef __cplusplus
/* C++ version: use template to handle integral and arbitrary‑precision types */
template <typename T>
static inline T next_value(const T *arr, idx_t i)
{
    idx_t n = i - 4;
    const T &a_n   = arr[i-4];
    const T &a_n1  = arr[i-3];
    const T &a_n2  = arr[i-2];
    const T &a_n3  = arr[i-1];

    const unsigned int c1 = (unsigned int)(n + 1);
    const unsigned int c2 = 3u * (unsigned int)(n + 3);
    const unsigned int c3 = 3u * (unsigned int)(n + 3) * (unsigned int)(n + 2);
    const unsigned int c4 = (unsigned int)(n + 3) * (unsigned int)(n + 2) * (unsigned int)(n + 1);

    if constexpr (std::is_integral_v<T>) {
        __int128 t1 = (__int128)a_n3 * c1;
        __int128 t2 = (__int128)a_n2 * c2;
        __int128 t3 = (__int128)a_n1 * c3;
        __int128 t4 = (__int128)a_n   * c4;
        return (T)(t4 + t3 + t2 - t1);
    } else {
        return a_n * c4 + a_n1 * c3 + a_n2 * c2 - a_n3 * c1;
    }
}
#else
/* C version: use signed __int128 for safe intermediate arithmetic */
static inline seq_t next_value(const seq_t *arr, idx_t i)
{
    idx_t n = i - 4;
    seq_t a_n   = arr[i-4];
    seq_t a_n1  = arr[i-3];
    seq_t a_n2  = arr[i-2];
    seq_t a_n3  = arr[i-1];

    unsigned int c1 = (unsigned int)(n + 1);
    unsigned int c2 = 3u * (unsigned int)(n + 3);
    unsigned int c3 = 3u * (unsigned int)(n + 3) * (unsigned int)(n + 2);
    unsigned int c4 = (unsigned int)(n + 3) * (unsigned int)(n + 2) * (unsigned int)(n + 1);

    signed __int128 t1 = (__int128)a_n3 * c1;
    signed __int128 t2 = (__int128)a_n2 * c2;
    signed __int128 t3 = (__int128)a_n1 * c3;
    signed __int128 t4 = (__int128)a_n   * c4;
    signed __int128 res = t4 + t3 + t2 - t1;
    return (seq_t)res;
}
#endif

/* public function: a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_len = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)18;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) continue;          /* already set */
        cache[i] = next_value(cache, i);
    }
    if ((size_t)n + 1 > cache_len) cache_len = (size_t)n + 1;

    return cache[n];
}
