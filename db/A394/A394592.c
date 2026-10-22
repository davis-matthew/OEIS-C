/*
 * A394592  Sum of the distinct prime divisors of C(5*n, n).
 * https://oeis.org/A394592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of binomial(5*n, n)
 * Range:     u8 n <= 12, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure that primes up to 'limit' are generated and stored in prime_list */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 as per the sequence definition */
    idx_t five_n = 5 * n;
    idx_t four_n = 4 * n;

    ensure_prime_limit(five_n);

    seq_t sum = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > five_n)
            break;

        /* Compute exponent of p in binomial(5n, n) */
        idx_t exp = 0;
        idx_t power = p;
        while (power <= five_n) {
            exp += five_n / power;
            exp -= n / power;
            exp -= four_n / power;

            /* Prevent overflow in the next multiplication */
            if (power > five_n / p)
                break;
            power *= p;
        }

        if (exp > 0)
            sum += (seq_t)p;
    }

    return sum;
}
