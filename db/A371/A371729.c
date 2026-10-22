/*
 * A371729  The number of pseudoprimes to base n that are smaller than n.
 * https://oeis.org/A371729
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ m < n : m composite and n^(m-1) ≡ 1 (mod m) }|
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/_floor_isqrt_u64.h"

static bool *prime_cache = NULL;
static idx_t prime_cache_limit = 0;

/* Build a prime boolean sieve up to 'limit' (inclusive). */
static void build_prime_sieve(idx_t limit)
{
    if (limit <= prime_cache_limit)
        return;

    /* Allocate and initialise new cache */
    bool *new_cache = (bool*)malloc((size_t)(limit + 1));
    if (!new_cache)
        return; /* Inadequate memory – unlikely for n <= 10000 */

    for (size_t i = 0; i <= (size_t)limit; ++i) {
        new_cache[i] = true;
    }
    if (limit >= 0) new_cache[0] = false;
    if (limit >= 1) new_cache[1] = false;

    idx_t sq = _floor_isqrt_u64(limit);
    for (idx_t i = 2; i <= sq; ++i) {
        if (new_cache[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_cache[j] = false;
        }
    }

    /* Replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Ensure the sieve covers all m < n */
    build_prime_sieve(n - 1);

    seq_t count = 0;
    for (idx_t m = 2; m < n; ++m) {
        if (prime_cache[m])
            continue;                 /* Skip primes */

        idx_t base_mod = n % m;
        seq_t res = mod_pow((seq_t)base_mod, (uint64_t)(m - 1), (seq_t)m);
        if (res == (seq_t)1)
            ++count;
    }

    return count;
}
