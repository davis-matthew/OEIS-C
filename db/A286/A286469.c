/*
 * A286469  a(n) = maximum of {the index of least prime dividing n} and {the
 *          maximal gap between indices of the successive primes in the prime
 *          factorization of n}.
 * https://oeis.org/A286469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A055396(n), A286470(n))
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum of the index of the least prime dividing n
   and the maximal gap between indices of successive prime factors of n.
   The function is thread‑safe for successive calls; prime tables are
   built only once and expanded on demand. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Utility prototypes */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Prime tables and mapping: prime_value → 1‑based index */
static uint64_t *primes      = NULL;   /* primes[0 .. prime_cnt-1]   */
static size_t   prime_cnt    = 0;      /* number of primes stored   */
static idx_t    primes_limit = 0;     /* maximum prime value stored */
static uint32_t *prime_index = NULL;   /* prime_index[p] = index of prime p (1‑based) */
static size_t   prime_index_size = 0;   /* size of prime_index array (limit+1) */

/* Ensure the primes up to at least `limit` (inclusive) are available. */
static void
ensure_primes(idx_t limit)
{
    if (primes == NULL || limit > primes_limit) {
        /* Free old structures if they exist */
        free(primes);
        free(prime_index);

        /* Build a new prime list up to `limit` */
        build_primes_upto(limit, &primes, &prime_cnt);
        primes_limit = limit;

        /* Allocate mapping array */
        prime_index_size = (size_t)(limit + 1);
        prime_index = (uint32_t*)calloc(prime_index_size, sizeof(uint32_t));
        if (prime_index == NULL) {
            /* Out‑of‑memory; abort the program (not expected in tests) */
            exit(EXIT_FAILURE);
        }

        /* Fill the mapping: prime_value → its 1‑based index */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p <= limit) {
                prime_index[(size_t)p] = (uint32_t)(i + 1);
            }
        }
    }
}

/* Compute a(n) for n >= 1 */
seq_t
a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 by definition */
        return (seq_t)0;

    /* Ensure prime data cover at least n (the largest possible prime factor) */
    ensure_primes(n);

    /* Factorize n into distinct primes (ascending order) */
    uint64_t factors[32];          /* enough for n <= 10000 */
    uint64_t exps[32];             /* exponents, not needed here */
    size_t factor_cnt = 0;
    factor_u64(n, factors, exps, &factor_cnt);

    if (factor_cnt == 0)          /* should not happen for n > 1 */
        return (seq_t)0;

    /* Index of the least prime factor (first factor) */
    uint32_t least_idx = prime_index[(size_t)factors[0]];

    /* Compute the maximal gap between indices of successive distinct prime factors */
    uint32_t max_gap = 0;
    uint32_t prev_idx = least_idx;  /* index of factors[0] */

    for (size_t i = 1; i < factor_cnt; ++i) {
        uint32_t cur_idx = prime_index[(size_t)factors[i]];
        uint32_t gap = cur_idx - prev_idx;
        if (gap > max_gap)
            max_gap = gap;
        prev_idx = cur_idx;
    }

    /* Final result is the maximum of the two contributions */
    uint32_t result = (least_idx > max_gap) ? least_idx : max_gap;
    return (seq_t)result;
}
