/*
 * A387274  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(n+4,k+4) *
 *          binomial(2*k+8,k+8).
 * https://oeis.org/A387274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387274:  a(n)=∑_{k=0..n} 2^(n‑k)·C(n+4,k+4)·C(2k+8,k+8)
   Recurrence (n>1):
      n*(n+8)*a(n) = (n+4)*(4*(2n+7)*a(n‑1) – 12*(n+3)*a(n‑2))
   Initial values: a(0)=1, a(1)=20
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* static memoisation cache */
    static seq_t *cache    = NULL;
    static idx_t  cache_sz = 0;
    static idx_t  cache_cap= 0;

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz  = 0;
    }
    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* set base values */
    if (cache_sz == 0) {
        cache[0] = (seq_t)1;
        cache_sz = 1;
    }
    if (cache_sz == 1) {
        cache[1] = (seq_t)20;
        cache_sz = 2;
    }

    /* whether seq_t fits in 64 bits – then use 128‑bit intermediate */
    const bool small = (sizeof(seq_t) <= 8);

    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t a_prev  = cache[i-1];
        seq_t a_prev2 = cache[i-2];
        seq_t ai;

        if (small) {
            /* 128‑bit arithmetic to avoid overflow for 64‑bit seq_t */
            unsigned __int128 term1 = (unsigned __int128)(2*i + 7) * (unsigned __int128)a_prev * 4u;
            unsigned __int128 term2 = (unsigned __int128)(i + 3) * (unsigned __int128)a_prev2 * 12u;
            unsigned __int128 diff  = term1 - term2;
            unsigned __int128 numer = (unsigned __int128)(i + 4) * diff;
            unsigned __int128 denom = (unsigned __int128)i * (unsigned __int128)(i + 8);
            ai = (seq_t)(numer / denom);
        } else {
            /* big‑integer path – native operators handle arbitrary width */
            seq_t term1 = a_prev;
            term1 *= (2*i + 7);
            term1 *= 4;
            seq_t term2 = a_prev2;
            term2 *= (i + 3);
            term2 *= 12;
            seq_t diff = term1 - term2;
            seq_t numer = diff * (i + 4);
            seq_t denom = (i) * (i + 8);
            ai = numer / denom;
        }

        cache[i] = ai;
    }

    cache_sz = n + 1;
    return cache[n];
}
