/*
 * A323896  Sum of binary Gray code A003188 and its Dirichlet inverse,
 *          A323895.
 * https://oeis.org/A323896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003188(n) + A323895(n) * where A003188(n) = n XOR (n >>
 *            1) * and A323895(n) is the Dirichlet inverse of A003188
 * Range:     i8 n <= 33, i16 n <= 1649, i32 n <= 8744, i64 n <= 8744,
 *            i128 n <= 8744, big any n
 * Helpers:   gray_code
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gray_code.h"

/* static storage for the Dirichlet inverse values */
static seq_t *inv_cache   = NULL;   /* cache[n] holds g(n) */
static bool  *inv_done    = NULL;   /* true if g(n) computed */
static idx_t cache_cap    = 0;       /* allocated size of the arrays */

/* enlarge the cache if necessary; all new entries are marked as
   not computed and have value 0. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(n + 1) <= cache_cap)
        return;

    idx_t new_cap = (n + 1) * 2;
    seq_t *new_cache = (seq_t *)realloc(inv_cache, new_cap * sizeof(seq_t));
    bool   *new_done  = (bool   *)realloc(inv_done , new_cap * sizeof(bool));

    if (!new_cache || !new_done) {
        /* allocation failed – abort the program (build system will handle) */
        abort();
    }

    for (idx_t i = cache_cap; i < new_cap; ++i) {
        new_cache[i] = 0;
        new_done[i]  = false;
    }

    inv_cache = new_cache;
    inv_done  = new_done;
    cache_cap = new_cap;
}

/* Recursive computation of the Dirichlet inverse of the Gray code.
   g(1) = 1, and for n > 1: g(n) = - Σ_{d|n, 1<d≤n} f(d)*g(n/d),
   where f(d) is the Gray code of d. */
static seq_t dirichlet_inverse(idx_t n)
{
    if (n == 0)
        return 0;                 /* never used, just to silence warnings */

    if (n == 1) {
        ensure_cache(1);
        if (!inv_done[1]) {
            inv_cache[1] = 1;
            inv_done[1]  = true;
        }
        return inv_cache[1];
    }

    ensure_cache(n);
    if (inv_done[n])
        return inv_cache[n];

    seq_t sum = gray_code(n);    /* term f(n)*g(1) = f(n) */

    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            /* term with divisor d */
            sum += gray_code(d) * dirichlet_inverse(e);

            if (e != d) {
                /* term with paired divisor e */
                sum += gray_code(e) * dirichlet_inverse(d);
            }
        }
    }

    seq_t g = -sum;
    inv_cache[n] = g;
    inv_done[n]  = true;
    return g;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    seq_t f = gray_code(n);
    seq_t g = dirichlet_inverse(n);
    return f + g;
}
