/*
 * A346623  a(n) = Sum_{ x <= n : x odd and omega(x) = 2 } x.
 * https://oeis.org/A346623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{x \le n : x \text{ odd and }\omega(x)=2} x (sum of
 *            all odd numbers \le n having exactly two distinct prime factors)
 * Range:     u8 n <= 54, u16 n <= 698, u32 n <= 9691, u64 n <= 9691,
 *            u128 n <= 9691, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* static storage for the sieve and prime list */
static uint64_t *prime_list = NULL;  /* list of primes up to current limit */
static size_t prime_count = 0;       /* number of primes in prime_list */
static idx_t max_limit = 0;          /* maximum n for which the primes were built */

/* Build the list of primes up to 'limit' (inclusive) using a
   simple Sieve of Eratosthenes.  This function keeps only the
   prime list in memory. */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_limit)
        return;

    /* Sieve bitmap: true means prime */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return;          /* memory failure – unlikely */

    memset(is_prime, true, sz);
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++count;

    /* Allocate prime list */
    uint64_t *new_primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            new_primes[pos++] = (uint64_t)i;

    free(is_prime);

    /* Replace old data */
    free(prime_list);
    prime_list = new_primes;
    prime_count = count;
    max_limit = limit;
}

/* Main sequence function.  It works for any n >= 1, in any order of calls. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* no odd composite with two distinct primes below 3 */

    ensure_primes(n);

    const seq_t N = (seq_t)n;   /* cast once for comparison */
    seq_t sum = 0;

    /* iterate over all odd primes p < q */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)            /* skip even prime */
            continue;
        if ((seq_t)p > N)
            break;

        seq_t p_pow = (seq_t)p;
        while (p_pow <= N) {
            /* For the current p_pow, q must satisfy q <= N / p_pow */
            seq_t limit_q = N / p_pow;
            for (size_t j = i + 1; j < prime_count; ++j) {
                uint64_t q = prime_list[j];
                if (q == 2)
                    continue;
                if ((seq_t)q > limit_q)
                    break;          /* further q are too large */

                seq_t q_pow = (seq_t)q;
                while (q_pow <= limit_q) {
                    seq_t prod = p_pow * q_pow;
                    if (prod > N)
                        break;
                    sum += prod;
                    if (q_pow > N / (seq_t)q)
                        break;
                    q_pow *= (seq_t)q;
                }
            }
            if (p_pow > N / (seq_t)p)
                break;
            p_pow *= (seq_t)p;
        }
    }

    return sum;
}
