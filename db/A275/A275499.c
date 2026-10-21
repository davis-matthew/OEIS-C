/*
 * A275499  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - 4*a(n-4) + 3*a(n-5) +
 *            a(n-6) - a(n-7) Initial values: a(1)=5, a(2)=54, a(3)=80,
 *            a(4)=136, a(5)=232, a(6)=516, a(7)=1168, a(8)=2660, a(9)=6024,
 *            a(10)=13716, a(11)=31168. Recurrence valid for n>11
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 52, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Initial 11 values */
    static const seq_t init[11] = {
        (seq_t)5,  (seq_t)54,  (seq_t)80,  (seq_t)136,
        (seq_t)232, (seq_t)516, (seq_t)1168, (seq_t)2660,
        (seq_t)6024,(seq_t)13716,(seq_t)31168
    };

    if (n <= 11) {
        return init[n-1];
    }

#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic resizing */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.assign(init, init + 11);   /* cache[0] == a(1) */
    }

    size_t old_size = cache.size();
    if ((size_t)n > old_size) {
        cache.resize(n);    /* ensure space for a(n) */
        for (idx_t i = old_size + 1; i <= n; ++i) {   /* compute a(i) */
            size_t idx = (size_t)i - 1;                /* 0‑based index for a(i) */
            seq_t a_nm1 = cache[idx-1];   /* a(i-1) */
            seq_t a_nm2 = cache[idx-2];   /* a(i-2) */
            seq_t a_nm3 = cache[idx-3];   /* a(i-3) */
            seq_t a_nm4 = cache[idx-4];   /* a(i-4) */
            seq_t a_nm5 = cache[idx-5];   /* a(i-5) */
            seq_t a_nm6 = cache[idx-6];   /* a(i-6) */
            seq_t a_nm7 = cache[idx-7];   /* a(i-7) */

            seq_t pos = a_nm1 * 2 + a_nm2 * 2 + a_nm5 * 3 + a_nm6;
            seq_t neg = a_nm3 * 2 + a_nm4 * 4 + a_nm7;
            cache[idx] = pos - neg;
        }
    }
    return cache[(size_t)n - 1];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (1‑based) */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache_len == 0) {   /* first call: initialise with the first 11 values */
        cache_cap = 11;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
        cache_len = 11;
    }

    if ((size_t)n > cache_cap) {   /* grow the buffer */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {   /* compute missing terms */
        size_t idx = (size_t)i - 1;
        seq_t a_nm1 = cache[idx-1];
        seq_t a_nm2 = cache[idx-2];
        seq_t a_nm3 = cache[idx-3];
        seq_t a_nm4 = cache[idx-4];
        seq_t a_nm5 = cache[idx-5];
        seq_t a_nm6 = cache[idx-6];
        seq_t a_nm7 = cache[idx-7];

        seq_t pos = a_nm1 * 2 + a_nm2 * 2 + a_nm5 * 3 + a_nm6;
        seq_t neg = a_nm3 * 2 + a_nm4 * 4 + a_nm7;
        cache[idx] = pos - neg;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
#endif
}
