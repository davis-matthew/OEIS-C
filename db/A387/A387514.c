/*
 * A387514  Number of ways to tile a fixed circular strip of length n with
 *          squares and dominos of two different colors where there must be
 *          the same number of each color of domino.
 * https://oeis.org/A387514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=1, a(4)=5 * For n ≥ 5: a(n) =
 *            ((2*n^2-7*n+4)*a(n-1) - (n^2-4*n+4)*a(n-2) +
 *            (4*n^2-20*n+24)*a(n-4)) / (n^2-3*n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Protect n = 0 – undefined but harmless */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1), ... */
    static size_t cap   = 0;      /* allocated size for cache  */
    static idx_t  last_computed = 0;   /* highest n that has been computed */

    /* Ensure we have enough room for n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure   */
        cache = tmp;
        /* No need to initialise new entries – we track last_computed */
        cap = new_cap;
    }

    /* If we already computed up to n, return it */
    if (n <= last_computed) return cache[n];

    /* Compute values up to n iteratively */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1 || i == 2 || i == 3) {
            cache[i] = 1;
        } else if (i == 4) {
            cache[i] = 5;
        } else {
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm2 = cache[i - 2];
            seq_t a_nm4 = cache[i - 4];

            seq_t N  = (seq_t)i;
            seq_t N2 = N * N;

            seq_t den   = N2 - 3 * N;                      /* n^2-3n  */
            seq_t term1 = (2 * N2 - 7 * N + 4) * a_nm1;     /* (2n^2-7n+4)a(n-1) */
            seq_t term2 = (N2 - 4 * N + 4) * a_nm2;         /* (n^2-4n+4)a(n-2) */
            seq_t term3 = (4 * N2 - 20 * N + 24) * a_nm4;   /* (4n^2-20n+24)a(n-4) */

            seq_t num = term1 + term3 - term2;
            cache[i] = num / den;   /* exact division as per formula */
        }
    }
    last_computed = n;
    return cache[n];
}
