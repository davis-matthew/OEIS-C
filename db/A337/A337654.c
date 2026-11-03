/*
 * A337654  Expansion of H(x)*(1+x^5)/(1-x^2-x^3-x^4) where H(x) = g.f. for
 *          A249665.
 * https://oeis.org/A337654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=10, a(6)=22,
 *            a(7)=45 Recurrence for n>7: a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3)
 *            + a(n-4) + a(n-5) - a(n-7) - a(n-8)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 120,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    /* Static memoisation buffer */
    static seq_t *buf = NULL;
    static size_t cap = 0;    /* total allocated capacity */
    static size_t len = 0;    /* number of computed terms (next free index) */

    /* Ensure buffer exists */
    if (buf == NULL) {
        cap = 16;
        buf = seq_alloc(cap);
    }

    /* Grow buffer to accommodate at least n+1 entries */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        buf = seq_realloc(buf, cap, newcap);
        cap = newcap;
    }

    /* Pre‑populate first eight terms if not already done */
    static const seq_t init[8] = {0,1,1,2,4,10,22,45};
    while (len < 8 && len <= (size_t)n) {
        buf[len] = init[len];
        ++len;
    }

    /* Compute missing terms using the recurrence */
    while (len <= (size_t)n) {
        seq_t val = 2 * buf[len-1];          /* 2*a(n-1) */
        val -= buf[len-2];                   /* - a(n-2) */
        val += 2 * buf[len-3];               /* + 2*a(n-3) */
        val += buf[len-4];                   /* + a(n-4) */
        val += buf[len-5];                   /* + a(n-5) */
        val -= buf[len-7];                   /* - a(n-7) */
        val -= buf[len-8];                   /* - a(n-8) */
        buf[len] = val;
        ++len;
    }

    return buf[n];
}
