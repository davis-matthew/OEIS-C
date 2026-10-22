/*
 * A322976  Number of divisors d of n such that d+2 is prime.
 * https://oeis.org/A322976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d+2 is prime
 * Range:     u8 n <= 9800, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define PRIME_LIMIT 70000U  /* enough for sqrt(n)+2 up to ~3·10^9 */

static bool sieve_initialized = false;
static unsigned int primes[PRIME_LIMIT + 1];
static size_t prime_count = 0;
static bool composite[PRIME_LIMIT + 1] = { false };

static void init_primes(void)
{
    if (sieve_initialized)
        return;

    composite[0] = composite[1] = true;
    for (unsigned int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                composite[j] = true;
        }
    }

    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i)
        if (!composite[i])
            primes[prime_count++] = i;

    sieve_initialized = true;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < prime_count; ++i) {
        unsigned int p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0)
        return res;                /* not used for n>=1 but guard anyway */

    init_primes();

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (is_prime_uint64((uint64_t)(d + 2)))
                res += 1;
            idx_t other = n / d;
            if (other != d) {
                if (is_prime_uint64((uint64_t)(other + 2)))
                    res += 1;
            }
        }
    }
    return res;
}
