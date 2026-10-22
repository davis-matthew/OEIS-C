/*
 * A369316  Number of Dyck bridges with resets to zero from (0,0) to (n,0).
 * https://oeis.org/A369316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = (4*(2*n-5)*a(n-2) + 4*(n-1)*a(n-3) - 16*(n-4)*a(n-4) -
 *            16*(n-4)*a(n-5))/(n-1) with initial values a(0)=1, a(1)=0,
 *            a(2)=2, a(3)=2, a(4)=8 (valid for all n ≥ 0)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* C++ build – assume seq_t is an arithmetic type or a big‑integer class.
   All operations are performed with seq_t directly. */
seq_t a(idx_t n) {
    /* static memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 4;                    /* index of last computed value */

    /* initial cache setup (only once) */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)8;
    }

    /* if requested value already computed, return it */
    if (n <= last) return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms (C++ build uses seq_t throughout) */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 2] * (seq_t)(4 * (2 * i - 5));
        seq_t term2 = cache[i - 3] * (seq_t)(4 * (i - 1));
        seq_t term3 = cache[i - 4] * (seq_t)(16 * (i - 4));
        seq_t term4 = cache[i - 5] * (seq_t)(16 * (i - 4));
        seq_t num = term1 + term2 - term3 - term4;
        seq_t den = (seq_t)(i - 1);
        cache[i] = num / den;
    }

    last = n;
    return cache[n];
}
#else
/* C build – seq_t is an unsigned integer type (uint8_t … uint64_t, unsigned __int128)
   Use unsigned __int128 for intermediate arithmetic to avoid overflow. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 4;

    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)8;
    }

    if (n <= last) return cache[n];

    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = last + 1; i <= n; ++i) {
        unsigned __int128 mul1 = (unsigned __int128)(4 * (2 * i - 5));
        unsigned __int128 mul2 = (unsigned __int128)(4 * (i - 1));
        unsigned __int128 mul3 = (unsigned __int128)(16 * (i - 4));

        unsigned __int128 term1 = mul1 * (unsigned __int128)cache[i - 2];
        unsigned __int128 term2 = mul2 * (unsigned __int128)cache[i - 3];
        unsigned __int128 term3 = mul3 * (unsigned __int128)cache[i - 4];
        unsigned __int128 term4 = mul3 * (unsigned __int128)cache[i - 5];

        unsigned __int128 num = term1 + term2 - term3 - term4;
        unsigned __int128 den = (unsigned __int128)(i - 1);

        cache[i] = (seq_t)(num / den);
    }

    last = n;
    return cache[n];
}
#endif
