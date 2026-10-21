/*
 * A248663  Binary encoding of the prime factors of the squarefree part of n.
 * https://oeis.org/A248663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0. For n > 1, let n = ∏ p_i^{e_i}. For each prime factor
 *            p_i whose exponent e_i is odd, XOR 2^{π(p_i)-1} into the result,
 *            where π(p_i) is the number of primes ≤ p_i. This gives the
 *            binary encoding of the prime factors of the squarefree part of n
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"          /* factor_u64 */
#include "../utils/build_primes_upto.h"  /* build_primes_upto */

static uint64_t *prime_cache = NULL; /* sorted primes up to cache_cap */
static size_t   prime_cache_cnt = 0;
static idx_t    prime_cache_cap  = 0;

/* ensure that prime_cache contains all primes ≤ limit */
static void
ensure_prime_cache(idx_t limit)
{
    if (limit <= prime_cache_cap)
        return;

    uint64_t *tmp = NULL;
    size_t   tmp_cnt = 0;
    build_primes_upto(limit, &tmp, &tmp_cnt);

    free(prime_cache);
    prime_cache      = tmp;
    prime_cache_cnt  = tmp_cnt;
    prime_cache_cap  = limit;
}

/* binary search for prime p in prime_cache, which is sorted.
   Returns its zero‑based index.  The prime must exist. */
static size_t
index_of_prime(uint64_t p)
{
    size_t low  = 0;
    size_t high = prime_cache_cnt;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        uint64_t val = prime_cache[mid];
        if (val == p)
            return mid;
        if (val < p)
            low = mid + 1;
        else
            high = mid;
    }
    /* Should never reach here for valid inputs. */
    return (size_t)(-1);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* factor n and keep exponents */
    uint64_t primes_of_n[64];
    uint64_t exponents[64];
    size_t   factor_cnt = 0;
    factor_u64((uint64_t)n, primes_of_n, exponents, &factor_cnt);

    /* ensure we have all primes up to n for index lookup */
    ensure_prime_cache(n);

    seq_t result = 0;

    for (size_t i = 0; i < factor_cnt; ++i) {
        if ((exponents[i] & 1) == 0)   /* even exponent -> skip */
            continue;

        uint64_t p = primes_of_n[i];
        size_t   idx = index_of_prime(p);  /* zero‑based index */

        /* 2^(idx) in seq_t */
        seq_t bit = ((seq_t)1 << (idx_t)idx);
        result ^= bit;
    }

    return result;
}
