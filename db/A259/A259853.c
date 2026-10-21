/*
 * A259853  Denominators of the terms of Lehmer's series S_2(2), where S_k(x)
 *          = Sum_{n>=1} n^k*x^n/binomial(2*n, n).
 * https://oeis.org/A259853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(n^2 * 2^n / binomial(2n,n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator equals binomial(2n,n) divided by the gcd of n^2 * 2^n and binomial(2n,n). */
/* This implementation computes the denominator by prime factorisation and Legendre's theorem, */
/* avoiding manipulation of gigantic intermediate binomial values. */

#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/pow_int.h"

static size_t factorial_exp(idx_t m, idx_t p)
{
    size_t e = 0;
    idx_t div = p;
    while (div <= m)
    {
        e += m / div;
        div *= p;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range; not used by the OEIS entry */

    /* Ensure we have all primes and SPF up to 2n */
    static idx_t max_limit = 0;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t *spf = NULL;

    idx_t limit = 2 * n;
    if (limit > max_limit)
    {
        /* Free previous allocations if needed */
        if (primes) free(primes);
        if (spf) free(spf);

        /* Build new prime list and SPF table */
        build_primes_upto(limit, &primes, &prime_cnt);
        build_spf(limit, &spf);
        max_limit = limit;
    }

    /* Factorise n using the SPF table */
    idx_t factors[64];
    size_t  fac_exp[64];
    size_t  fac_cnt = 0;
    idx_t x = n;
    while (x > 1)
    {
        idx_t p = spf[x];
        size_t e = 0;
        while (x % p == 0)
        {
            x /= p;
            ++e;
        }
        factors[fac_cnt]  = p;
        fac_exp[fac_cnt]  = e;
        ++fac_cnt;
    }

    seq_t denom = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];
        size_t e_np = 0;          /* exponent of p in n */
        for (size_t j = 0; j < fac_cnt; ++j)
            if (factors[j] == p)
            {
                e_np = fac_exp[j];
                break;
            }

        size_t e_B = factorial_exp(2 * n, p) - 2 * factorial_exp(n, p); /* exponent in binomial(2n,n) */

        size_t e_num = 0;
        if (p == 2)
            e_num = 2 * e_np + n;   /* exponent of 2 in n^2 * 2^n */
        else
            e_num = 2 * e_np;       /* exponent for odd primes */

        size_t extra = (e_B > e_num) ? (e_B - e_num) : 0;
        if (extra == 0)
            continue;

        if (extra == 1)
            denom = denom * (seq_t)p;              /* fast path */
        else
            denom = denom * pow_int((seq_t)p, (idx_t)extra);
    }

    return denom;
}
