/*
 * A290186  Expansion of (1+x)/ ((1+x)^3-7*x).
 * https://oeis.org/A290186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A290186 - Expanding (1+x)/((1+x)^3-7x) a(0)=1, a(1)=5, a(2)=17
 *            a(n)=4*a(n-1)-3*a(n-2)-a(n-3) for n>=3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored values */
    static size_t  cap   = 0;     /* array capacity */
    static idx_t  len    = 0;     /* next unchecked index (so cache[0..len-1] are valid) */

    /* initial values */
    const seq_t init[3] = {1, 5, 17};

    /* first call: allocate and fill the three initial terms */
    if (len == 0) {
        cap = 4;                         /* start a bit larger than needed */
        cache = seq_alloc(cap);
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        len = 3;
    }

    /* enlarge array if required */
    if ((idx_t)len <= n) {
        size_t needed = (size_t)(n + 1);
        if (cap < needed) {
            size_t newcap = cap;
            while (newcap < needed) newcap <<= 1;
            cache = seq_realloc(cache, cap, newcap);
            cap = newcap;
        }
        /* compute missing terms up to n */
        for (idx_t i = len; i <= n; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            cache[i] = 4*t1 - (3*t2 + t3);
        }
        len = n + 1;
    }

    return cache[(size_t)n];
}
