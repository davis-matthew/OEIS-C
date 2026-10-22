/*
 * A395612  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(4*n,n+2*k).
 * https://oeis.org/A395612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*binomial(4*n,n+2*k) Recurrence
 *            (a(0)=1, a(1)=8): 3*n*(3*n-2)*(3*n-1)*(28*n^2-77*n+52)*a(n) =
 *            16*(1456*n^5-6188*n^4+9690*n^3-6844*n^2+2105*n-210)*a(n-1) +
 *            64*(n-1)*(4*n-7)*(4*n-5)*(28*n^2-21*n+3)*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   denom_int128, denom_seq_t, poly_p1_int128, poly_p1_seq_t,
 *            poly_p2_int128, poly_p2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/denom_int128.h"
#include "../utils/denom_seq_t.h"
#include "../utils/poly_p1_int128.h"
#include "../utils/poly_p1_seq_t.h"
#include "../utils/poly_p2_int128.h"
#include "../utils/poly_p2_seq_t.h"

static seq_t *cache = NULL;     /* cache of computed a(n) values   */
static size_t cache_cap = 0;    /* allocated capacity in cache     */
static idx_t last_computed = (idx_t)-1; /* highest n already computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* initialise cache on first use */
    if (last_computed == (idx_t)-1) {
        cache_cap = 2;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        last_computed = 1;
    }

    /* extend cache up to n */
    if (n > last_computed) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            if (new_cap < need) new_cap = need;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)8;
            } else {
                if (sizeof(seq_t) <= 8) {
                    /* use __int128 for intermediate calculations */
                    __int128 p1 = poly_p1_int128(i);
                    __int128 p2 = poly_p2_int128(i);
                    __int128 den = denom_int128(i);
                    __int128 num = (__int128)16 * p1 * (__int128)cache[i - 1]
                                 + (__int128)64 * p2 * (__int128)cache[i - 2];
                    cache[i] = (seq_t)(num / den);
                } else {
                    /* use seq_t arithmetic (for 128-bit or arbitrary‑precision) */
                    seq_t p1 = poly_p1_seq_t(i);
                    seq_t p2 = poly_p2_seq_t(i);
                    seq_t den = denom_seq_t(i);
                    seq_t num = (seq_t)16 * p1 * cache[i - 1]
                              + (seq_t)64 * p2 * cache[i - 2];
                    cache[i] = num / den;
                }
            }
        }
        last_computed = n;
    }

    return cache[n];
}
