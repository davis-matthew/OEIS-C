/*
 * A270992  Number of distinct prime divisors of prime(n)+1 and prime(n+1)+1.
 * https://oeis.org/A270992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of distinct prime divisors of prime(n)+1 and prime(n+1)+1
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, count_distinct_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"
#include "../utils/count_distinct_primes.h"

static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

/* Ensure that prime_list contains at least the first (n+1) primes */
static void
ensure_primes_up_to(idx_t n)
{
    /* Already have enough primes? */
    if (prime_cnt > (size_t)n)
        return;

    /* Upper bound for the (n+1)-th prime.
       For n ≥ 6, p_n < n (log n + log log n).
       We use a generous factor to simplify the estimate. */
    idx_t limit = (n + 1) * 20ull;
    if (limit < 100ull)
        limit = 100ull;

    if (prime_limit >= limit)
        return;

    uint64_t *new_primes = NULL;
    size_t   new_cnt     = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    if (prime_list != NULL)
        free(prime_list);

    prime_list  = new_primes;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* sequence is defined for n >= 1 */

    /* Ensure we have prime(n) and prime(n+1) available */
    ensure_primes_up_to(n);

    uint64_t p = prime_list[n - 1];  /* prime(n)   */
    uint64_t q = prime_list[n];      /* prime(n+1) */

    seq_t p_plus = (seq_t)p + 1;
    seq_t q_plus = (seq_t)q + 1;

    /* Greatest common divisor of (p+1) and (q+1) */
    seq_t g = gcd(p_plus, q_plus);

    /* Count distinct prime divisors of the gcd */
    uint64_t g64 = (uint64_t)g;               /* g fits in 64 bits for the required n */
    seq_t result = (seq_t)count_distinct_primes(g64);

    return result;
}
