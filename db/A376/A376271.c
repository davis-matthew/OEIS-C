/*
 * A376271  Numbers k such that there exists at least one proper divisor that
 *          is neither squarefree nor a prime power, i.e., m is in A126706.
 * https://oeis.org/A376271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that there exists at least one proper divisor
 *            that is neither squarefree nor a prime power, equivalent to: k
 *            satisfies bigomega(k) > omega(k) > 1 and (omega(k) >= 3 or
 *            (omega(k)=2 and one exponent >=3). Implementation enumerates all
 *            integers and keeps those that satisfy the conditi...
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

/* Helper: test whether n satisfies the condition for the sequence */
static bool is_candidate(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    if (cnt <= 1)                /* not composite or only one distinct prime */
        return false;

    uint64_t omega = cnt;        /* number of distinct primes */
    uint64_t big   = 0;          /* total number of prime factors with multiplicity */
    for (size_t i = 0; i < cnt; ++i)
        big += ex[i];

    if (big <= omega)            /* bigomega <= omega -- not in A033987 */
        return false;

    if (omega >= 3)              /* k has at least three distinct primes, guaranteed a proper divisor exists */
        return true;

    /* omega == 2 : check exponents */
    /* The only failing pattern is (2,1) or (1,2) (p^2*q or p*q^2) */
    if ((ex[0] == 2 && ex[1] == 1) || (ex[0] == 1 && ex[1] == 2))
        return false;

    return true;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of computed terms */
    static size_t cache_cap = 0;   /* capacity of the cache array */
    static size_t cache_len = 0;   /* number of terms stored */
    static uint64_t next_test = 2; /* next integer to test for being a member */

    /* extend cache until it contains the n-th term */
    while (cache_len < (size_t)n)
    {
        if (is_candidate(next_test))
        {
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
                if (!new_ptr) /* OOM protection – return zero */
                    return (seq_t)0;
                if (cache)
                {
                    memcpy(new_ptr, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache    = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
        }
        ++next_test;
    }

    return cache[n - 1];
}
