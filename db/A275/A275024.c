/*
 * A275024  Total weight of the n-th twice-prime-factored multiset partition.
 * https://oeis.org/A275024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e‖n} e * Ω(index(p)+1) (Ω = Ω(m), the total number
 *            of prime factors of m counting multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9310 (2026-11-02)
 * Helpers:   bigomega, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *prime_list = NULL;   /* array of primes, 0‑based */
static size_t   primes_cnt = 0;        /* number of primes in array   */
static uint64_t prime_limit = 0;       /* largest prime in array      */

/* make sure the prime list contains all primes ≤ limit */
static void ensure_primes_limit(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list)
        free(prime_list);
    prime_list = new_list;
    primes_cnt = new_cnt;
    prime_limit = limit;
}

/* binary search for the index of prime p in prime_list */
static size_t prime_index(uint64_t p)
{
    if (p > prime_limit)
        ensure_primes_limit(p);        /* prime_list now covers p          */

    size_t lo = 0, hi = primes_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return mid;
        else if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* should never happen for a prime factor */
    return (size_t)(-1);
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t un = (uint64_t)n;

    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt = 0;
    factor_u64(un, primes_f, exps_f, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes_f[i];
        size_t idx = prime_index(p);                  /* 0‑based index   */
        seq_t weight = bigomega((idx_t)(idx + 2));     /* Ω(index+1)     */
        sum += weight * (seq_t)exps_f[i];
    }

    return sum;
}
