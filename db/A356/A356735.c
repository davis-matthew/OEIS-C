/*
 * A356735  Number of distinct parts that have neighbors in the integer
 *          partition with Heinz number n.
 * https://oeis.org/A356735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8664, u16 n <= 8664, u32 n <= 8664, u64 n <= 8664,
 *            u128 n <= 8664, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct prime indices of n that have
 * adjacent prime indices also dividing n.
 * Implementation: factor n into distinct primes, determine each prime's
 * index by a linear search in the global prime table, then count
 * indices having a neighbor.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *global_primes = NULL;
static size_t   global_prime_cnt = 0;
static idx_t    prime_table_limit = 0;

/* Ensure we have all primes up to at least `limit`. The table is
 * rebuilt only if a larger limit is requested. */
static void
ensure_prime_table(idx_t limit)
{
    if (prime_table_limit >= limit)
        return;

    if (global_primes != NULL) {
        free(global_primes);
        global_primes = NULL;
    }
    build_primes_upto(limit, &global_primes, &global_prime_cnt);
    prime_table_limit = limit;
}

/* Linear search for the 1‑based index of prime `p`.  It is assumed
 * that `p` is in the global prime table. */
static uint32_t
prime_index(uint64_t p)
{
    for (size_t i = 0; i < global_prime_cnt; ++i)
        if (global_primes[i] == p)
            return (uint32_t)(i + 1);    /* indices start at 1 */
    return 0;   /* should never happen for a prime divisor of n */
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;    /* 1 has no prime factors */

    /* Prepare prime table for all primes up to n.  Needed for index lookup. */
    ensure_prime_table(n);

    /* Get distinct prime factors of n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Mark which prime indices appear.  Allocate a contiguous boolean array. */
    bool *present = (bool *)calloc(global_prime_cnt + 2, sizeof(bool));
    if (present == NULL)
        return (seq_t)0;   /* allocation failure – unlikely for small n */

    for (size_t i = 0; i < cnt; ++i) {
        uint32_t idx = prime_index(primes[i]);   /* 1‑based index */
        if (idx > 0 && idx <= global_prime_cnt)
            present[idx] = true;
    }

    /* Count indices that have a neighbor. */
    seq_t result = 0;
    for (size_t i = 1; i <= global_prime_cnt; ++i) {
        if (!present[i])
            continue;
        bool has_neighbor = false;
        if (i > 1 && present[i - 1])
            has_neighbor = true;
        else if (i < global_prime_cnt && present[i + 1])
            has_neighbor = true;
        if (has_neighbor)
            ++result;
    }

    free(present);
    return result;
}
