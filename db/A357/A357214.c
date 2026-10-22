/*
 * A357214  a(n) = number of subsets S of {1, 2, ..., n} such that every
 *          number in S is a composite.
 * https://oeis.org/A357214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n - π(n) - 1) (number of composites ≤ n)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It counts subsets of {1,…,n} whose elements are all composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Global sieve state, shared across calls.  */
static idx_t          sieve_limit = 0;
static bool          *sieve_comp  = NULL;   /* Composite marker array. */
static uint64_t      *primes      = NULL;   /* Prime list. */
static size_t         prime_count = 0;      /* Number of primes found. */

/* Expand the sieve until it covers at least 'limit'. */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(sieve_comp);
    free(primes);

    sieve_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve_comp)
        exit(1);

    /* Allocate maximum possible space for primes (worst case). */
    primes = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!primes)
        exit(1);

    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve_comp[i]) {
            primes[pc++] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve_comp[j] = true;
            }
        }
    }
    prime_count = pc;
    sieve_limit = limit;
}

/* Return π(n) – the number of primes ≤ n – by binary search over the
   locally stored prime list. */
static inline size_t _prime_pi(idx_t n)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  */
    _ensure_sieve(n);

    size_t pc = _prime_pi(n);          /* π(n) */
    idx_t composite_cnt = (n > pc + 1 ? n - pc - 1 : 0);  /* composites ≤ n */

    return pow2((idx_t)composite_cnt);
}
