/*
 * A346483  Sum of A005171 (characteristic function of nonprimes) and its
 *          Dirichlet inverse.
 * https://oeis.org/A346483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A005171(n) + A346482(n) (Dirichlet inverse of the
 *            nonprime characteristic function). A005171(n) = 1 if n is 1 or
 *            composite, 0 if n is prime. Recurrence for the Dirichlet inverse
 *            g: g(1) = 1 g(n) = -A005171(n) - Σ_{d|n, 1<d<n} A005171(d) *
 *            g(n/d) for n>1 Then a(n) = A005171(n) + g(n)
 * Range:     i8 n <= 8639, i16 n <= 9039, i32 n <= 9039, i64 n <= 9039,
 *            i128 n <= 9039, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

static seq_t *g_cache = NULL;   /* Dirichlet inverse values */
static size_t g_cap   = 0;       /* current allocation size */
static idx_t  g_max   = 0;       /* highest n for which g is known */

/* characteristic function of nonprimes (nonprime(1)=1) */
static inline seq_t nonprime(idx_t n)
{
    return (n == 1 || !is_prime(n)) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for 0, return 0 */
        return 0;

    /* initialise cache on first call */
    if (g_cache == NULL) {
#if defined(__cplusplus)
        g_cache = new seq_t[2];
#else
        g_cache = (seq_t*)malloc(sizeof(seq_t) * 2);
#endif
        g_cache[0] = 0;          /* unused */
        g_cache[1] = 1;          /* Dirichlet inverse at 1 */
        g_cap   = 2;
        g_max   = 1;
    }

    /* grow cache if required */
    if (n > g_max) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > g_cap) {
#if defined(__cplusplus)
            seq_t *newarr = new seq_t[new_cap];
            for (size_t i = 0; i <= g_max; ++i) newarr[i] = g_cache[i];
            delete[] g_cache;
            g_cache = newarr;
#else
            g_cache = (seq_t*)realloc(g_cache, new_cap * sizeof(seq_t));
#endif
            g_cap = new_cap;
        }

        for (idx_t k = g_max + 1; k <= n; ++k) {
            seq_t fn = nonprime(k);
            seq_t sum = 0;
            idx_t limit = int_sqrt(k);
            for (idx_t d = 2; d <= limit; ++d) {
                if (k % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = k / d;

                    if (nonprime(d1))
                        sum += g_cache[d2];
                    if (d2 != d1 && nonprime(d2))
                        sum += g_cache[d1];
                }
            }
            g_cache[k] = -fn - sum;   /* Dirichlet inverse recurrence */
        }
        g_max = n;
    }

    seq_t fn = nonprime(n);
    return fn + g_cache[n];           /* a(n) = f(n) + g(n) */
}
