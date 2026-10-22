/*
 * A369460  Number of representations of 12n-9 as a sum (p*q + p*r + q*r) with
 *          three odd primes p <= q <= r.
 * https://oeis.org/A369460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 12*n-9 as p*q + p*r + q*r
 *            with odd primes p ≤ q ≤ r
 * Range:     u8 n <= 9297, u16 n <= 9297, u32 n <= 9297, u64 n <= 9297,
 *            u128 n <= 9297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SIEVE_MAX 20000            /* sufficient for n ≤ 10000 */

/* static state for sieve */
static bool sieve_initialized = false;
static bool is_prime[SIEVE_MAX + 1];
static uint32_t primes[3000];
static size_t primes_cnt = 0;

/* build smallest prime sieve once */
static void init_sieve(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;

    for (size_t i = 0; i <= SIEVE_MAX; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* even numbers >2 are composite */
    for (size_t i = 4; i <= SIEVE_MAX; i += 2) is_prime[i] = false;

    /* odd primes */
    for (size_t i = 3; i * i <= SIEVE_MAX; i += 2) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_MAX; j += 2 * i)
                is_prime[j] = false;
        }
    }

    /* store prime list (all primes up to SIEVE_MAX) */
    for (size_t i = 2; i <= SIEVE_MAX; ++i) {
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
    }
}

seq_t a(idx_t n)
{
    init_sieve();

    /*
     * target = 12*n - 9
     * (use idx_t for the arithmetic; it is the same type as the input)
     */
    idx_t target = 12 * n - 9;
    seq_t count = 0;

    /* iterate over odd primes p */
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint32_t p = primes[ip];
        if ((p & 1U) == 0) continue;           /* skip 2 */
        if ((uint64_t)p * p > target) break;    /* p ≤ sqrt(target) */

        /* iterate over odd primes q ≥ p */
        for (size_t iq = ip; iq < primes_cnt; ++iq) {
            uint32_t q = primes[iq];
            if ((q & 1U) == 0) continue;         /* skip 2 */

            uint64_t pq = (uint64_t)p * q;
            if (pq > target) break;               /* further q are larger */

            uint64_t rem = (uint64_t)target - pq;
            if (rem == 0) continue;               /* r would be 0 */

            uint64_t denom = (uint64_t)p + q;
            if (rem % denom) continue;           /* r not integral */

            uint64_t r = rem / denom;
            if (r < q) continue;                 /* enforce p ≤ q ≤ r */

            if (r > SIEVE_MAX) continue;         /* r out of sieve range */

            if (is_prime[(size_t)r] && (r & 1U))
                ++count;
        }
    }

    return count;
}
