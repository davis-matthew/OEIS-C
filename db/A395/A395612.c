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
 * Validated: standard, pass, 34 terms through n = 594 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute denominator den(n) = 3*n*(3*n-2)*(3*n-1)*(28*n^2-77*n+52) */
static __int128 denom_int128(idx_t n)
{
    __int128 N = n;
    return (3 * N) * (3 * N - 2) * (3 * N - 1) *
           (28 * N * N - 77 * N + 52);
}

static seq_t denom_seq_t(idx_t n)
{
    seq_t ii = (seq_t)n;
    seq_t term1 = (seq_t)(3 * n);
    seq_t term2 = (seq_t)(3 * n - 2);
    seq_t term3 = (seq_t)(3 * n - 1);
    seq_t term4 = (seq_t)(28 * n * n - 77 * n + 52);
    return term1 * term2 * term3 * term4;
}

/* Helper: compute polynomial p1(n) = 1456*n^5 - 6188*n^4 + 9690*n^3 - 6844*n^2 + 2105*n - 210  */
static __int128 poly_p1_int128(idx_t n)
{
    __int128 N = n;
    return (__int128)1456 * N * N * N * N * N
         - (__int128)6188 * N * N * N * N
         + (__int128)9690 * N * N * N
         - (__int128)6844 * N * N
         + (__int128)2105 * N
         - (__int128)210;
}

static seq_t poly_p1_seq_t(idx_t n)
{
    seq_t ii = (seq_t)n;
    seq_t ii2 = ii * ii;
    seq_t ii3 = ii2 * ii;
    seq_t ii4 = ii3 * ii;
    seq_t ii5 = ii4 * ii;
    return (seq_t)1456 * ii5
         - (seq_t)6188 * ii4
         + (seq_t)9690 * ii3
         - (seq_t)6844 * ii2
         + (seq_t)2105 * ii
         - (seq_t)210;
}

/* Helper: compute polynomial p2(n) = (n-1)*(4*n-7)*(4*n-5)*(28*n^2-21*n+3) */
static __int128 poly_p2_int128(idx_t n)
{
    __int128 N = n;
    return (N - 1) * (4 * N - 7) * (4 * N - 5) *
           (28 * N * N - 21 * N + 3);
}

static seq_t poly_p2_seq_t(idx_t n)
{
    seq_t ii = (seq_t)n;
    seq_t term1 = (seq_t)(n - 1);
    seq_t term2 = (seq_t)(4 * n - 7);
    seq_t term3 = (seq_t)(4 * n - 5);
    seq_t term4 = (seq_t)(28 * n * n - 21 * n + 3);
    return term1 * term2 * term3 * term4;
}

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
        cache = mem_resize(seq_t, NULL, cache_cap);
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
            cache = mem_resize(seq_t, cache, new_cap);
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
