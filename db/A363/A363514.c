/*
 * A363514  Sum of divisors of 3*n-1 of form 3*k+1.
 * https://oeis.org/A363514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   accumulate_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363514:  a(n) = sum of divisors d of (3*n-1) with d ≡ 1 (mod 3). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/accumulate_divisors.h"

/* Prime table – built once on first call. */
static uint32_t primes[6000];
static size_t primecnt = 0;
static bool primes_inited = false;

static void
init_prime_table(void)
{
    if (primes_inited)
        return;
    const uint32_t LIMIT = 50000U;          /* plenty for n up to 10000 */
    bool sieve[LIMIT + 1];
    for (size_t i = 0; i <= LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint32_t p = 2U; (uint64_t)p * p <= LIMIT; ++p) {
        if (sieve[p]) {
            for (uint32_t k = p * p; k <= LIMIT; k += p)
                sieve[k] = false;
        }
    }
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            primes[primecnt++] = (uint32_t)i;
    primes_inited = true;
}

/* Main sequence function.  The implementation uses a factorisation of
   3*n-1 and then generates all divisors, summing those ≡ 1 (mod 3). */
seq_t
a(idx_t n)
{
    if (n == 0)               /* safety; the offset is 1 */
        return 0;

    init_prime_table();

    /* t = 3*n - 1 */
    uint64_t t = (uint64_t)3 * (uint64_t)n - 1ULL;
    uint64_t rem = t;

    /* factorisation storage – a 64‑bit number has at most 64 distinct primes. */
    uint32_t fac[64];
    uint8_t  exps[64];
    size_t   fcnt = 0;

    for (size_t i = 0; i < primecnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            uint8_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            fac[fcnt]   = p;
            exps[fcnt]  = e;
            ++fcnt;
        }
    }
    if (rem > 1) {                   /* remainder is a prime > sqrt(t) */
        fac[fcnt]   = (uint32_t)rem;
        exps[fcnt]  = 1;
        ++fcnt;
    }

    seq_t sum = 0;
    accumulate_divisors(1ULL, 0, fac, exps, fcnt, &sum);
    return sum;
}
