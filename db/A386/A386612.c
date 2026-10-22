/*
 * A386612  a(n) = Sum_{k=0..n-1} binomial(4*k+1,k) * binomial(4*n-4*k,n-k-1).
 * https://oeis.org/A386612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(4*k+1,k) *
 *            binomial(4*n-4*k,n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑order linear recurrence with polynomial coefficients used for fast
   computation.  The sequence is memoised for arbitrary call order.  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* a[0..last_computed] */
static idx_t  last_computed = 0;  /* index of highest cached value */

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    if (n <= last_computed)
        return cache[n];

    /* extend cache to hold up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute new values */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) { /* already set */
            cache[0] = (seq_t)0;
            continue;
        }
        if (i == 1) { /* already set */
            cache[1] = (seq_t)1;
            continue;
        }

        /* For builtin integer types use 128‑bit intermediates to avoid overflow.
           For arbitrary‑precision types use the seq_t arithmetic directly. */
#if defined(__SIZEOF_INT128__) || defined(__INT128__)
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit or smaller build – use unsigned __int128 for intermediates */
            __uint128_t ip  = (__uint128_t)i;
            __uint128_t ip2 = ip * ip;
            __uint128_t ip3 = ip2 * ip;
            __uint128_t ip4 = ip3 * ip;
            __uint128_t ip5 = ip4 * ip;

            __uint128_t LHS = (__uint128_t)81 * ip *
                              (3 * ip + 1) *
                              (3 * ip + 2) *
                              (264 * ip2 - 395 * ip + 139);

            __uint128_t RHS1 = (__uint128_t)24 *
                               (152064 * ip5 -
                                189504 * ip4 -
                                2976  * ip3 +
                                47566 * ip2 -
                                4595  * ip     -
                                1080);

            RHS1 = RHS1 * (__uint128_t)cache[i-1];

            __uint128_t RHS2 = (__uint128_t)2048 *
                               ((2 * ip - 1) *
                                (4 * ip - 5) *
                                (4 * ip - 3) *
                                (264 * ip2 + 133 * ip + 8));

            RHS2 = RHS2 * (__uint128_t)cache[i-2];

            __uint128_t numerator = RHS1 - RHS2;
            cache[i] = (seq_t)(numerator / LHS);
            continue;
        }
#endif

        /* Big‑integer build – all arithmetic uses seq_t */
        seq_t ip  = (seq_t)i;
        seq_t ip2 = ip * ip;
        seq_t ip3 = ip2 * ip;
        seq_t ip4 = ip3 * ip;
        seq_t ip5 = ip4 * ip;

        seq_t LHS = 81 * ip *
                    (3 * ip + 1) *
                    (3 * ip + 2) *
                    (264 * ip2 - 395 * ip + 139);

        seq_t RHS1 = 24 *
                     (152064 * ip5 -
                      189504 * ip4 -
                      2976  * ip3 +
                      47566 * ip2 -
                      4595  * ip -
                      1080);

        RHS1 = RHS1 * cache[i-1];

        seq_t RHS2 = 2048 *
                     ((2 * ip - 1) *
                      (4 * ip - 5) *
                      (4 * ip - 3) *
                      (264 * ip2 + 133 * ip + 8));

        RHS2 = RHS2 * cache[i-2];

        cache[i] = (RHS1 - RHS2) / LHS;
    }

    last_computed = n;
    return cache[n];
}
