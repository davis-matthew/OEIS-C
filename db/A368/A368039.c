/*
 * A368039  The product of exponents of prime factorization of the
 *          nonsquarefree numbers.
 * https://oeis.org/A368039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005361(A013929(n)) * A013929(n) gives the nth
 *            nonsquarefree number (numbers divisible by a square) *
 *            A005361(k) is the product of the exponents in the prime
 *            factorization of k. * The function computes the nth term of
 *            A368039 by iterating over integers, * selecting the
 *            nonsquarefree ones,...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of terms stored                      */
    static size_t cache_cap  = 0;   /* allocated capacity                           */
    /* next integer to test for being nonsquarefree */
    static uint64_t next_candidate = 4;   /* 1,2,3 are squarefree */

    if (n == 0)     /* offset starts at 1; behave sensibly for n==0 */
        return 0;

    /* Grow cache as needed */
    if (cache_size < (size_t)n) {
        if (cache_cap < (size_t)n)
            ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* tmp buffers for factorization */
        static uint64_t pr[32];
        static uint64_t ex[32];
        size_t cnt;

        /* compute terms until we reach index n */
        while (cache_size < (size_t)n) {
            uint64_t candidate = next_candidate;
            while (1) {
                /* factor candidate */
                factor_u64(candidate, pr, ex, &cnt);
                seq_t prod = 1;
                for (size_t i = 0; i < cnt; ++i)
                    prod *= (seq_t)ex[i];

                /* candidate is nonsquarefree iff product > 1 */
                if (prod > 1) {
                    cache[cache_size++] = prod;
                    next_candidate = candidate + 1;   /* advance search start */
                    break;
                }
                ++candidate;  /* check next integer */
            }
        }
    }
    return cache[n - 1];
}
