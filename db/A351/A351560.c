/*
 * A351560  a(n) is a binary representation of the primes that divide sigma(n)
 *          [the sum of divisors of n function], shown in decimal.
 * https://oeis.org/A351560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = integer whose binary digits are 1 for each prime dividing
 *            σ(n) (σ is the sum–of–divisors function); the bits are numbered
 *            0,1,2,… with bit k set iff the (k+1)-th prime (2,3,5,…) divides
 *            σ(n)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 276, u64 n <= 660,
 *            u128 n <= 728, big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utilities */
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

/* Global storage for primes up to a fixed limit. */
static uint64_t *primes = NULL;     /* array of primes (ascending order)   */
static size_t   prime_cnt = 0;      /* number of primes in the array        */
static int      primes_initialized = 0;

/* Initialise the primes table once. Limit is chosen well above the size of
   σ(n) for n up to 10 000 (σ(10000)=24 211). */
static void init_primes(void)
{
    if (primes_initialized) return;

    const idx_t LIMIT = 200000;     /* safe upper bound for primes needed   */
    build_primes_upto(LIMIT, &primes, &prime_cnt);
    primes_initialized = 1;
}

/* Find the index of prime q in the global primes array.
   It is assumed that q is prime and appears in the array. */
static size_t prime_index(uint64_t q)
{
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        if (primes[i] == q) return i;
    }
    /* In theory we should never get here; return 0 as a safe fallback. */
    return 0;
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    init_primes();

    if (n < 1) return (seq_t)0;         /* undefined for n=0, just return 0 */

    /* Factor n with the prime table. Store at most 16 distinct primes
       to keep the auxiliary arrays small (n ≤ 10 000 guarantees this). */
    uint64_t pf[16];
    size_t   pev[16];
    size_t   pf_cnt = 0;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i)
    {
        if (m % primes[i] == 0)
        {
            uint64_t p = primes[i];
            size_t   e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pf[pf_cnt]   = p;
            pev[pf_cnt] = (size_t)e;
            ++pf_cnt;
        }
    }
    if (m > 1)                 /* remaining prime factor */
    {
        pf[pf_cnt]   = m;
        pev[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Compute σ(n) = ∏ (p^(e+1)-1)/(p-1). */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < pf_cnt; ++i)
    {
        uint64_t p = pf[i];
        size_t   e = pev[i];
        seq_t    term = (pow_uint(p, (idx_t)(e + 1)) - (seq_t)1) / ((seq_t)(p - 1));
        sigma *= term;
    }

    /* Factor σ(n) by repeated trial division using the same prime table.
       σ(n) fits in 64 bits for all n ≤ 10 000 (actually σ(10000)=24 211). */
    uint64_t sigma_val = (uint64_t)sigma;
    seq_t    result = (seq_t)0;
    uint64_t temp   = sigma_val;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i)
    {
        uint64_t q = primes[i];
        if (temp % q == 0)
        {
            size_t idx = prime_index(q);          /* index of this prime   */
            result |= ((seq_t)1u << idx);        /* set the corresponding bit   */
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1)                /* the remaining factor is prime > √σ(n) */
    {
        size_t idx = prime_index(temp);
        result |= ((seq_t)1u << idx);
    }

    return result;
}
