/*
 * A356225  Number of divisors of n whose prime indices do not cover an
 *          initial interval of positive integers.
 * https://oeis.org/A356225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) - A356224(n) * where A356224(n) is the number
 *            of divisors of n whose prime indices * form an initial interval
 *            of positive integers (including 1). The divisor * 1 is also
 *            excluded from this count
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/compute_pi.h"

/* The index type (uint64_t) and sequence type are supplied by the build. */
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt    = 0;
static bool     primes_ready = false;

/* Initialise the prime table once.  A limit of 2 000 000 covers all
 * primes needed for factoring numbers up to roughly 10^12.  This is more
 * than enough for the intended test ranges. */
static void init_primes(void)
{
    if (primes_ready)
        return;
    /* Build all primes up to SIEVE_LIMIT; the primes will be sorted increasingly. */
    const idx_t SIEVE_LIMIT = 2000000;
    build_primes_upto(SIEVE_LIMIT, &prime_list, &prime_cnt);
    primes_ready = true;
}

/* Return true if p divides n.  Used for computing the length of the
 * initial prime prefix present in n. */
static inline bool divides(idx_t n, uint64_t p)
{
    return (n % p) == 0;
}

/* Factor n.  The caller provides arrays of sufficient length.  Returns
 * the number of distinct prime factors.  The factorisation is performed
 * by trial division over the prime table built in init_primes. */
static size_t factorize(uint64_t n,
                        uint64_t *primes_out,
                        int *exps_out)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i]* (uint64_t)prime_list[i] <= tmp; ++i)
    {
        if (tmp % prime_list[i] == 0)
        {
            int e = 0;
            while (tmp % prime_list[i] == 0)
            {
                tmp /= prime_list[i];
                ++e;
            }
            primes_out[cnt] = prime_list[i];
            exps_out[cnt]  = e;
            ++cnt;
        }
    }
    if (tmp > 1)           /* remaining factor is prime > sqrt(original n) */
    {
        primes_out[cnt] = tmp;
        exps_out[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1; below offset return 0 */
        return 0;

    init_primes();

    /* Factor n. */
    uint64_t factors[64];
    int exps[64];
    size_t pcnt = factorize((uint64_t)n, factors, exps);

    /* Total number of divisors: product (e_i + 1). */
    seq_t d = 1;
    for (size_t i = 0; i < pcnt; ++i)
        d *= (seq_t)(exps[i] + 1);

    /* Find t: longest prefix of primes that all divide n. */
    idx_t t = 0;
    for (idx_t k = 1; k <= prime_cnt && k <= pcnt; ++k)
    {
        if (divides(n, prime_list[k-1]))
            ++t;
        else
            break;
    }

    /* Count divisors whose prime indices form an initial interval.
     * The divisor 1 is excluded here. */
    seq_t sum = 1;          /* divisor 1 */
    seq_t pref_prod = 1;    /* product of exponents e_1 ... e_k */
    for (idx_t k = 1; k <= t; ++k)
    {
        pref_prod *= (seq_t)exps[k-1];
        sum += pref_prod;
    }

    return d - sum;
}
