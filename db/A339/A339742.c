/*
 * A339742  Number of factorizations of n into distinct primes or squarefree
 *          semiprimes.
 * https://oeis.org/A339742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2969, u16 n <= 9246, u32 n <= 9246, u64 n <= 9246,
 *            u128 n <= 9246, big any n
 * Helpers:   count_distinct_primes, is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A339742 – Number of factorizations of n into distinct primes or squarefree semiprimes.
    The algorithm enumerates factorizations with increasing factors (to enforce distinctness),
    restricting each chosen factor to be either a prime or a squarefree semiprime.
    It uses recursion without memoisation – the search space is small for n ≤ 10000. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/is_squarefree.h"

static seq_t rec(uint64_t n, uint64_t min_factor)
{
    if (n < 2) return 0;
    seq_t total = 0;

    /* single‑factor case: allowed only if n itself is a prime or a semiprime */
    if (is_prime((idx_t)n) ||
        (is_squarefree((seq_t)n) && count_distinct_primes(n) == 2))
        total += 1;

    /* integer sqrt of n, computed by integer loop */
    uint64_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n) ++limit;

    /* try all potential factors f in increasing order, starting at min_factor */
    for (uint64_t f = min_factor; f <= limit; ++f) {
        if (n % f != 0) continue;
        if (f <= 1) continue;
        /* f must be a prime or a squarefree semiprime */
        if (!(is_prime((idx_t)f) ||
              (is_squarefree((seq_t)f) && count_distinct_primes(f) == 2)))
            continue;
        uint64_t q = n / f;
        if (q < f + 1) continue;          /* ensures distinctness */
        total += rec(q, f + 1);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;                /* a(1) = 1 by definition */
    return rec((uint64_t)n, 2);
}
