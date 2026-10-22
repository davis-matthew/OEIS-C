/*
 * A325224  Sum of prime indices of n minus the lesser of the number of prime
 *          factors of n counted with multiplicity and the maximum prime index
 *          of n.
 * https://oeis.org/A325224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_i index(p_i)*e_i - min(total_factors, max_index)
 *            where n = ∏ p_i^{e_i}
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

/* Helper headers (names match the function names). */
#include "../utils/factor_u64.h"
#include "../utils/build_primes.h"

/* Static cache of primes up to the largest n seen. */
static idx_t *prime_list = NULL;   /* array of prime numbers */
static size_t prime_cnt  = 0;      /* number of primes in prime_list */
static idx_t  prime_limit = 0;     /* current maximum prime pre‑computed */

/* Ensure that prime_list contains all primes ≤ limit. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free previously allocated list, if any. */
    if (prime_list)
        free(prime_list);

    /* Build a new list of primes ≤ limit. */
    build_primes(limit, &prime_list, &prime_cnt);

    prime_limit = limit;
}

/* Binary search to find the 1‑based index of prime p (guaranteed to exist). */
static idx_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* prime_list[lo] must equal p. */
    assert(lo < prime_cnt && prime_list[lo] == p);
    return (idx_t)(lo + 1);          /* indices start at 1 */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n = 0, return 0 */

    /* Ensure we can look up prime indices for all prime factors of n. */
    ensure_primes_up_to(n);

    /* Temporary arrays for factorization: enough for 64‑bit n. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_indices = 0;          /* Σ index(p_i) * e_i */
    seq_t total_facs  = 0;          /* Σ e_i */
    seq_t max_index   = 0;          /* max index(p_i) */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t   p     = (idx_t)primes[i];
        seq_t   e     = (seq_t)exps[i];
        idx_t   idx_p = prime_index(p);

        sum_indices += idx_p * e;
        total_facs  += e;
        if (idx_p > max_index)
            max_index = idx_p;
    }

    /* a(n) = sum_indices - min(total_facs, max_index) */
    seq_t sub = (total_facs < max_index) ? total_facs : max_index;
    return sum_indices - sub;
}
