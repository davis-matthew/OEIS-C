/*
 * A393242  a(n) is the number of divisors of n! with exactly n divisors.
 * https://oeis.org/A393242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n! that have exactly n divisors
 * Range:     u8 n <= 23, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   prime_exp_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/prime_exp_in_factorial.h"

/* ---------- prime sieve (static) ---------- */

#define MAX_PRIME_LIMIT 200000

static uint32_t primes[MAX_PRIME_LIMIT];
static size_t primes_count = 0;
static bool primes_built = false;

/* simple sieve of Eratosthenes, fills the global array `primes` */
static void build_primes(void)
{
    if (primes_built)
        return;

    const size_t limit = MAX_PRIME_LIMIT - 1;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return; /* failure: trust that subsequent calls will not need primes */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[primes_count++] = (uint32_t)i;

    free(is_prime);
    primes_built = true;
}

/* --------- exponent of prime p in n! (Legendre) --------- */

/* -------- recursive counting -------------------------------------- */

/*
 * Recursive helper:
 *   idx      : current prime index
 *   T        : remaining product needed (starts as n)
 *   exps     : array of exponents of primes in n! (size = exps_count)
 *   exps_count: number of primes considered
 */
static seq_t dfs_rec(size_t idx, idx_t T, uint64_t *exps, size_t exps_count)
{
    if (T == (idx_t)1)
        return (seq_t)1;                  /* all remaining primes can be 0 */

    if (idx == exps_count)
        return (T == (idx_t)1 ? (seq_t)1 : (seq_t)0);

    seq_t total = dfs_rec(idx + 1, T, exps, exps_count); /* e = 0 */

    uint64_t a_exp = exps[idx];          /* exponent of current prime in n! */

    /* iterate over all divisors d of T, d > 1 */
    for (idx_t d = 1; d * d <= T; ++d)
    {
        if (T % d == 0)
        {
            idx_t other = T / d;

            /* use divisor d as (e+1) */
            if (d > 1)
            {
                uint64_t e = (uint64_t)d - 1;
                if (e <= a_exp)
                    total += dfs_rec(idx + 1, other, exps, exps_count);
            }

            /* use the complementary divisor 'other' as (e+1) */
            if (other != d && other > 1)
            {
                uint64_t e = (uint64_t)other - 1;
                if (e <= a_exp)
                    total += dfs_rec(idx + 1, d, exps, exps_count);
            }
        }
    }

    return total;
}

/* ---------- public function a(idx_t n) --------------------------- */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* ensure primes are available */
    build_primes();

    /* count primes <= n */
    size_t count_primes = 0;
    while (count_primes < primes_count && (idx_t)primes[count_primes] <= n)
        ++count_primes;

    /* allocate array of exponents for these primes */
    uint64_t *exps = (uint64_t *)malloc(count_primes * sizeof(uint64_t));
    if (!exps)          /* allocation failure – return 0 as a safe fallback */
        return (seq_t)0;

    for (size_t i = 0; i < count_primes; ++i)
        exps[i] = prime_exp_in_factorial((uint64_t)n, primes[i]);

    /* compute answer */
    seq_t result = dfs_rec(0, n, exps, count_primes);

    free(exps);
    return result;
}
