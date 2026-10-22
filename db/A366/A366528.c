/*
 * A366528  Sum of odd prime indices of n.
 * https://oeis.org/A366528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd prime indices of n * * The implementation
 *            pre‑computes all primes up to 10000, along with * their 1‑based
 *            indices. For each factor p of n (with multiplicity * exp) the
 *            odd indices are accumulated: sum += idx(p)*exp. Only * primes up
 *            to 10000 are required because the intended dom...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* static storage for the prime sieve (up to 10000) */
#define PRIME_LIMIT 10000U

static bool       primes_inited = false;
static uint8_t    is_prime[PRIME_LIMIT + 1];
static uint32_t   primes[PRIME_LIMIT + 1];
static uint16_t   prime_index[PRIME_LIMIT + 1];
static size_t     prime_cnt = 0;

/* initialise the prime sieve once */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    /* mark all as prime initially, then clear 0 and 1 */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = 0; is_prime[1] = 0;

    /* classic Eratosthenes sieve */
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* record primes and their 1‑based indices */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt]      = (uint32_t)i;
            prime_index[i]        = (uint16_t)(prime_cnt + 1);
            ++prime_cnt;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* offset starts at 1; 0 and negative indices are undefined */
    if (n < 1) return 0;
    if (n == 1) return 0;          /* a(1) = 0 by definition */

    init_primes();                 /* make sure the sieve is ready */

    seq_t sum = 0;
    uint64_t temp = (uint64_t)n;   /* work on a 64‑bit copy */

    /* trial division using the pre‑computed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t     p  = primes[i];
        uint64_t     pp = (uint64_t)p * p;
        if (pp > temp) break;          /* remaining part, if any, is prime */

        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            uint16_t idx = prime_index[p];
            if ((idx & 1U) != 0)        /* odd index */
                sum += (seq_t)idx * (seq_t)exp;
        }
    }

    /* remaining prime factor, if > 1 */
    if (temp > 1) {
        /* temp is a prime > sqrt(original n).  It must be ≤ 10000
           for the intended domain. */
        if (temp <= PRIME_LIMIT) {
            uint16_t idx = prime_index[(size_t)temp];
            if ((idx & 1U) != 0)
                sum += (seq_t)idx;    /* exponent is 1 */
        }
    }

    return sum;
}
