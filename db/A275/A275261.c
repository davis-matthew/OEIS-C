/*
 * A275261  Number of nX3 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=54, a(3)=128, a(4)=396, a(5)=1440, a(6)=5676,
   a(7)=22844, a(8)=93968, a(9)=389820, a(10)=1626348, a(11)=6799728,
   a(12)=28470780, a(13)=119289068, a(14)=500005184, a(15)=2096208716,
   a(16)=8789088828, a(17)=36853587936; for n>17
   a(n)=6*a(n-1)-4*a(n-2)-20*a(n-3)+28*a(n-4)-40*a(n-5)+8*a(n-6)
        +194*a(n-7)-180*a(n-8)+36*a(n-9)+56*a(n-10)-356*a(n-11)
        +201*a(n-12)+180*a(n-13)-108*a(n-14). */
#include <stdlib.h>

static const seq_t init_terms[17] = {
    (seq_t)5,
    (seq_t)54,
    (seq_t)128,
    (seq_t)396,
    (seq_t)1440,
    (seq_t)5676,
    (seq_t)22844,
    (seq_t)93968,
    (seq_t)389820,
    (seq_t)1626348,
    (seq_t)6799728,
    (seq_t)28470780,
    (seq_t)119289068,
    (seq_t)500005184,
    (seq_t)2096208716,
    (seq_t)8789088828,
    (seq_t)36853587936
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* Undefined for n<1, return 0 for safety */

    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;
    static size_t capacity = 0;

    if (n <= cached_upto) {
        return cache[n - 1];
    }

    /* Grow cache if necessary */
    if ((idx_t)capacity < n) {
        size_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* Allocation failure – abort silently (behavior undefined per spec) */
            exit(1);
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing entries */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        idx_t idx = i - 1;               /* zero‑based index */
        seq_t val;
        if (i <= 17) {
            val = init_terms[idx];
        } else {
            val = 6 * cache[idx - 1]           /* a(n-1) */
                - 4 * cache[idx - 2]           /* a(n-2) */
                - 20 * cache[idx - 3]          /* a(n-3) */
                + 28 * cache[idx - 4]          /* a(n-4) */
                - 40 * cache[idx - 5]          /* a(n-5) */
                + 8 * cache[idx - 6]           /* a(n-6) */
                + 194 * cache[idx - 7]         /* a(n-7) */
                - 180 * cache[idx - 8]         /* a(n-8) */
                + 36 * cache[idx - 9]          /* a(n-9) */
                + 56 * cache[idx - 10]         /* a(n-10) */
                - 356 * cache[idx - 11]        /* a(n-11) */
                + 201 * cache[idx - 12]        /* a(n-12) */
                + 180 * cache[idx - 13]        /* a(n-13) */
                - 108 * cache[idx - 14];       /* a(n-14) */
        }
        cache[idx] = val;
    }

    cached_upto = n;
    return cache[n - 1];
}
