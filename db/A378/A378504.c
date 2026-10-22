/*
 * A378504  Expansion of (Sum_{k>=0} binomial(3*k,k) * x^k)^4.
 * https://oeis.org/A378504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378504
 *
 * a(n) = Sum_{i+j+k+l=n} binomial(3*i,i) * binomial(3*j,j) *
 *        binomial(3*k,k) * binomial(3*l,l)
 *
 * Recurrence (valid for n >= 2):
 *     8*(n-1)*n*(2*n-1)*a(n) =
 *         6*(n-1)*(36*n^2 - 9*n - 5)*a(n-1)
 *       - 81*n*(3*n-2)*(3*n-1)*a(n-2)
 *
 * Base values: a(0) = 1, a(1) = 12
 *
 * The function memoises results in a static cache so that calls
 * in any order are efficient.  It uses only arithmetic
 * operations on the user‑supplied integer type `seq_t`;
 * it works with 8‑, 16‑, 32‑, 64‑bit and arbitrary‑precision
 * integer types.  Intermediate calculations for 64‑bit types
 * use unsigned __int128 to avoid overflow.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    last  = (idx_t)-1;   /* last index that is initialized */

    /* Ensure capacity for indices up to n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise a(0) and a(1) if not already */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        last     = 0;
    }
    if (n >= 1 && last < 1) {
        cache[1] = (seq_t)12;
        last     = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; last = i; continue; }
        if (i == 1) { cache[i] = (seq_t)12; last = i; continue; }

        seq_t N  = (seq_t)i;          /* n as seq_t */
        seq_t n1 = N - 1;             /* n-1 */

        /* Handle 64‑bit types with 128‑bit intermediates to avoid overflow */
        if (sizeof(seq_t) <= 8) {
            /* Small types: use unsigned __int128 for intermediates */
            uint64_t N64  = (uint64_t)N;
            uint64_t n1_64 = (uint64_t)n1;
            uint64_t expr64 = 36ull * N64 * N64 - 9ull * N64 - 5ull;

            unsigned __int128 t1 = (unsigned __int128)cache[i-1];
            t1 *= 6;
            t1 *= n1_64;
            t1 *= expr64;

            unsigned __int128 t2 = (unsigned __int128)cache[i-2];
            t2 *= 81;
            t2 *= N64;
            t2 *= (3ull * N64 - 2ull);
            t2 *= (3ull * N64 - 1ull);

            unsigned __int128 num = t1 - t2;

            unsigned __int128 denom = 8ull * n1_64 * N64 * (2ull * N64 - 1ull);

            cache[i] = (seq_t)(num / denom);
        } else {
            /* Arbitrary‑precision or >64‑bit types: use seq_t operations */
            seq_t expr_big = 36 * N * N - 9 * N - 5;
            seq_t term1 = 6 * n1 * expr_big * cache[i-1];
            seq_t term2 = 81 * N * (3 * N - 2) * (3 * N - 1) * cache[i-2];
            seq_t denom = 8 * n1 * N * (2 * N - 1);
            cache[i] = (term1 - term2) / denom;
        }

        last = i;
    }

    return cache[n];
}
