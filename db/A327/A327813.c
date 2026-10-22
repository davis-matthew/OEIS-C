/*
 * A327813  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(4) (counted with multiplicity).
 * https://oeis.org/A327813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) / ord_4(odd part of n) where * n = 2^e * s, s odd,
 *            φ(n) is Euler's totient, * ord_4(s) is the multiplicative order
 *            of 4 modulo s
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"
#include "../utils/powmod64.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure the list of primes up to 10000 is built once. */
static void init_primes(void)
{
    if (primes == NULL)
        build_primes_upto(10000, &primes, &primes_cnt);
}

/* Euler's totient using the global prime list. */
static uint64_t phi_of(uint64_t m)
{
    uint64_t t = m, result = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= t; ++i)
    {
        uint64_t p = primes[i];
        if (t % p == 0)
        {
            /* count multiplicity */
            t /= p;
            while (t % p == 0)
                t /= p;
            result *= (p - 1);
            /* multiply p^(e-1) where e≥1; here we already removed one factor */
            uint64_t pp = 1;
            while (t % p == 0)  /* no effect because t removed all p's */
                pp *= p;
            /* but we need p^(e-1); e was counted by how many times we divided */
            /* Instead, recompute exponent by dividing original m */
            uint64_t tmp = m;
            size_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            for (size_t k = 1; k < e; ++k)
                result *= p;
        }
    }
    if (t > 1)  /* remaining prime factor */
        result *= (t - 1);
    return result;
}

/* Return the unique prime factors of m (stored in array pf). */
static size_t prime_factors_of(uint64_t m, uint64_t pf[], size_t max)
{
    size_t cnt = 0;
    uint64_t t = m;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= t; ++i)
    {
        uint64_t p = primes[i];
        if (t % p == 0)
        {
            if (cnt < max) pf[cnt++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1 && cnt < max) pf[cnt++] = t;
    return cnt;
}

seq_t a(idx_t n)
{
    init_primes();

    uint64_t nn = (uint64_t)n;
    /* isolate odd part s of n */
    uint64_t tmp = nn;
    while ((tmp & 1u) == 0) tmp >>= 1;
    uint64_t s = tmp;

    /* phi(n) */
    uint64_t phi_n = phi_of(nn);

    /* ord_4(s) */
    uint64_t ord;
    if (s == 1) {
        ord = 1;
    } else {
        /* compute phi(s) */
        uint64_t phi_s = phi_of(s);
        /* factor phi_s to get its prime divisors */
        uint64_t pf[64];
        size_t pf_cnt = prime_factors_of(phi_s, pf, 64);
        ord = phi_s;
        for (size_t i = 0; i < pf_cnt; ++i)
        {
            uint64_t p = pf[i];
            while (ord % p == 0)
            {
                uint64_t cand = ord / p;
                if (powmod64(4, cand, s) == 1)
                    ord = cand;
                else
                    break;
            }
        }
    }

    return (seq_t)(phi_n / ord);
}
