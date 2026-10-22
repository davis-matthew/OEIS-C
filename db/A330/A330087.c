/*
 * A330087  Permanent of a square matrix M(n) whose general element M_{i,j} is
 *          defined by i*prime(j).
 * https://oeis.org/A330087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * ∏_{k=1}^{n} p_k (a(0) = 1) where p_k is the k‑th
 *            prime. The permanent of the matrix M_{ij}=i*p_j is n! times the
 *            product of row entries times the product of column entries,
 *            yielding the above formula
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* cache array of computed permanents   */
    static size_t cache_cap = 0;    /* capacity of cache in elements        */
    static idx_t max_n     = 0;     /* highest index that has been computed */
    static seq_t fact      = 1;     /* factorial of max_n                   */
    static seq_t prime_prod= 1;     /* product of the first max_n primes     */

    /* initialise cache lazily (needed even for n == 0) */
    if (cache_cap == 0) {
        cache_cap = 1;                     /* at least one element for a(0)       */
        cache = seq_alloc(cache_cap);      /* NULL on out‑of‑memory               */
        if (cache == NULL) return 0;       /* return 0 as error sentinel          */
        cache[0] = (seq_t)1;               /* a(0) = 1                            */
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) return 0;                     /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n incrementally */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            fact *= (seq_t)i;                          /* factorial update   */
            prime_prod *= (seq_t)get_prime(i);         /* prime product update*/
            cache[i] = fact * fact * prime_prod;      /* permanent value    */
        }
        max_n = n;
    }

    return cache[n];
}
