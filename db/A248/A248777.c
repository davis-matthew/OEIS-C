/*
 * A248777  Greatest k such that k^8 divides n!
 * https://oeis.org/A248777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest k such that k^8 divides n!
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 97, u64 n <= 151,
 *            u128 n <= 259, big any n
 * Helpers:   pow_uint, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_primes.h"
#include "../utils/pow_uint.h"

static idx_t max_cached = 0;
static idx_t *primes = NULL;
static size_t prime_cnt = 0;

static void ensure_primes(idx_t n)
{
    if (n <= max_cached)
        return;
    free(primes);
    sieve_primes(n, &primes, &prime_cnt);
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 1; /* offset 1 */

    ensure_primes(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n)
            break;

        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p)
                break;
            power *= p;
        }

        idx_t exp = e / 8;
        if (exp)
            result *= pow_uint((uint64_t)p, exp);
    }

    return result;
}
