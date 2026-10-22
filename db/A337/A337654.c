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
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"   /* seq_alloc(cap) */
#include "../utils/seq_realloc.h" /* seq_realloc(old, old_cap, new_cap) */

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
