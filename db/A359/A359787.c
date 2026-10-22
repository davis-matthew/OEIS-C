/*
 * A359787  Parity of Dirichlet inverse of A075255, where A075255(n) = n -
 *          sopfr(n), where sopfr is the sum of prime factors (with
 *          repetition).
 * https://oeis.org/A359787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9779, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of the Dirichlet inverse of f(n) = n - sopfr(n)       */
/* f(n) is odd iff n and the parity of the sum of prime factors with repetition differ  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache data */
static uint8_t  *fpar   = NULL;         /* parity of f(n) */
static uint8_t  *gpar   = NULL;         /* parity of Dirichlet inverse g(n) */
static uint32_t *spf    = NULL;         /* smallest prime factor array */
static uint32_t *primes = NULL;         /* temporary prime list */
static size_t   prime_cnt = 0;          /* number of primes in list */
static idx_t    max_n   = 0;             /* largest n already pre‑computed */

/* build spf array up to limit (inclusive) using a linear sieve */
static void build_spf(idx_t limit)
{
    spf    = (uint32_t *)realloc(spf,    (limit + 1) * sizeof(uint32_t));
    primes = (uint32_t *)realloc(primes, (limit + 1) * sizeof(uint32_t));

    spf[0] = 0;
    spf[1] = 0;           /* unused later */

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            uint32_t p = primes[j];
            if (p > spf[i] || (idx_t)p * i > limit)
                break;
            spf[p * i] = p;
        }
    }
}

/* extend all pre‑computed tables up to n */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    /* allocate / grow parity arrays */
    fpar = (uint8_t *)realloc(fpar, (n + 1) * sizeof(uint8_t));
    gpar = (uint8_t *)realloc(gpar, (n + 1) * sizeof(uint8_t));

    /* build spf and primes up to n */
    build_spf(n);

    /* compute fpar[n] for all n up to the new limit */
    fpar[1] = 1;               /* f(1)=1, parity 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        uint8_t sop_parity = 0;
        idx_t  x = i;
        while (x > 1)
        {
            uint32_t p = spf[x];
            uint8_t  exp_parity = 0;
            while (x % p == 0)
            {
                x /= p;
                exp_parity ^= 1;
            }
            if (exp_parity)          /* odd exponent */
                sop_parity ^= (uint8_t)(p & 1);
        }
        fpar[i] = (uint8_t)((i & 1) ^ sop_parity);    /* (i + sop) mod 2 */
    }

    /* gpar[1] is always 1 */
    gpar[1] = 1;

    /* compute gpar[n] for the new numbers */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        if (i == 1)                /* already set */
            continue;
        uint8_t val = fpar[i];     /* start with f(i) (recurrence) */
        for (idx_t d = 2; d * d <= i; ++d)
        {
            if (i % d == 0)
            {
                idx_t od = i / d;
                val ^= (fpar[d] & gpar[od]);
                if (od != d)
                    val ^= (fpar[od] & gpar[d]);
            }
        }
        gpar[i] = val;
    }

    max_n = n;                    /* remember the new bound */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* undefined for n < 1, but return 0 */
    ensure_up_to(n);
    return (seq_t)gpar[n];
}
