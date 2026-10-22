/*
 * A307627  Primes p such that 2 is a primitive root modulo p while 8 is not.
 * https://oeis.org/A307627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p such that p is prime, 2 is a primitive root modulo p,
 *            and p ≡ 1 (mod 3)
 * Range:     u8 n <= 9, u16 n <= 982, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Test whether 2 is a primitive root modulo p.  p is assumed to be
   a prime that fits in uint64_t. */
static bool is_primitive_root_2(seq_t p)
{
    uint64_t m = (uint64_t)p - 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t q = primes[i];
        if (mod_pow((seq_t)2, m / q, p) == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static size_t term_capacity = 0;
    static uint64_t search_start = 3;  /* next odd candidate */

    if (n == 0)
        return 0;   /* undefined offset 0 */

    /* grow the sequence until the nth term is available */
    while ((size_t)n > term_count)
    {
        uint64_t cand = search_start;
        for (;; cand += 2)   /* only odd numbers */
        {
            if (!is_prime((idx_t)cand))
                continue;

            if ((cand % 3) != 1)
                continue;

            if (!is_primitive_root_2((seq_t)cand))
                continue;

            /* a qualifying prime was found */
            if (term_count == term_capacity)
            {
                size_t new_cap = term_capacity == 0 ? 64 : term_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);  /* memory error */
                terms = tmp;
                term_capacity = new_cap;
            }
            terms[term_count++] = (seq_t)cand;
            search_start = cand + 2;  /* next odd after this prime */
            break;   /* break out of the inner loop */
        }
    }
    return terms[n - 1];
}
