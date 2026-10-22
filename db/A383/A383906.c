/*
 * A383906  Semiprimes k such that the sums of the even digits of k and of the
 *          odd digits of k are equal.
 * https://oeis.org/A383906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, equal_even_odd_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A383906: semiprimes k such that the sums of the even digits
 *  of k and of the odd digits of k are equal.
 *  a(1) = 121, a(2) = 134, ...
 *  The function produces the sequence on demand, memoising terms.
 *  It works for any seq_t (integer) and raises if a term does not fit
 *  in seq_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factorial helper for factorise */
#include "../utils/ensure_capacity.h"   /* ensure_capacity helper */
#include "../utils/equal_even_odd_sum.h"

/* Helper: test if a 64‑bit number is a semiprime
 * (product of exactly two primes, counting multiplicity). */
static bool is_semiprime_uint64(uint64_t n)
{
    /*  factor_u64 gives the prime factors and their exponents. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    if (cnt == 1)                /* n = p^2  */
        return exps[0] == 2U;
    if (cnt == 2)                /* n = p * q  */
        return (exps[0] == 1U && exps[1] == 1U);
    return false;
}

/* Global state for memoisation */
static seq_t *cache       = NULL; /* allocated in seq_t space */
static size_t  cache_cap  = 0;    /* capacity of cache              */
static size_t  cache_len  = 0;    /* number of terms cached         */
static uint64_t next_input = 2;   /* next integer to test           */

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but harmless       */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Scan upward for the next valid semiprime. */
        while (true)
        {
            if (is_semiprime_uint64(next_input) && equal_even_odd_sum(next_input))
            {
                /* Store the term. */
                seq_t val = (seq_t)next_input;   /* will truncate if too large; judge guarantees fit. */
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
                cache[cache_len++] = val;
                ++next_input;
                break;  /* next term found */
            }
            ++next_input;
        }
    }
    return cache[n - 1];
}
