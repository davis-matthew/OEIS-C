/*
 * A329605  Number of divisors of A108951(n), where A108951 is fully
 *          multiplicative with a(prime(i)) = prime(i)# = Product_{i=1..i}
 *          A000040(i).
 * https://oeis.org/A329605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of A108951(n), where A108951 is fully
 *            multiplicative and A108951(prime(i)) = prime(i)#, the primorial
 *            of the i‑th prime
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- prime sieve ---------- */
#define PRIME_SIEVE_LIMIT  1000000  /* enough for all n in the tests */

static uint32_t *primes = NULL;   /* array of primes in ascending order */
static size_t   prime_cnt = 0;    /* number of primes */

static void build_primes(void)
{
    if (primes != NULL)
        return;          /* already built */

    static unsigned char is_comp[PRIME_SIEVE_LIMIT + 1];
    size_t estimate = PRIME_SIEVE_LIMIT / 10;            /* rough upper bound */
    primes = (uint32_t *)malloc(estimate * sizeof(uint32_t));
    if (!primes) exit(1);
    prime_cnt = 0;

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            if (prime_cnt >= estimate)
            {
                estimate *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, estimate * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT)
            {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    /* shrink to exact size */
    uint32_t *tmp = (uint32_t *)realloc(primes, prime_cnt * sizeof(uint32_t));
    if (tmp) primes = tmp;
}

/* binary search for index (1‑based) of prime p in primes[] */
static size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return mid + 1;   /* 1‑based index */
        else if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* p is not in precomputed list (should not happen for test data) */
    return 0;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) == 1, and safety for n==0 */
        return (seq_t)1;

    build_primes();

    /* factor n */
    uint64_t m = (uint64_t)n;
    /* dynamic arrays for factors */
    uint32_t *factor_indices = NULL;
    uint32_t *factor_exps    = NULL;
    size_t   fcnt = 0;
    size_t   fcap = 4;
    factor_indices = (uint32_t *)malloc(fcap * sizeof(uint32_t));
    factor_exps    = (uint32_t *)malloc(fcap * sizeof(uint32_t));
    if (!factor_indices || !factor_exps) exit(1);

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i)
    {
        uint32_t p = primes[i];
        if (m % p == 0)
        {
            uint32_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            if (fcnt >= fcap)
            {
                fcap *= 2;
                uint32_t *tmpi = (uint32_t *)realloc(factor_indices, fcap * sizeof(uint32_t));
                uint32_t *tpep = (uint32_t *)realloc(factor_exps,    fcap * sizeof(uint32_t));
                if (!tmpi || !tpep) exit(1);
                factor_indices = tmpi;
                factor_exps    = tpep;
            }
            factor_indices[fcnt] = prime_index(p);
            factor_exps[fcnt]    = e;
            ++fcnt;
        }
    }
    if (m > 1)   /* remaining prime factor > sqrt(original n) */
    {
        uint32_t p = (uint32_t)m;
        if (fcnt >= fcap)
        {
            fcap *= 2;
            uint32_t *tmpi = (uint32_t *)realloc(factor_indices, fcap * sizeof(uint32_t));
            uint32_t *tpep = (uint32_t *)realloc(factor_exps,    fcap * sizeof(uint32_t));
            if (!tmpi || !tpep) exit(1);
            factor_indices = tmpi;
            factor_exps    = tpep;
        }
        factor_indices[fcnt] = prime_index(p);
        factor_exps[fcnt]    = 1;
        ++fcnt;
    }

    if (fcnt == 0)   /* n == 1, handled above but guard */
    {
        free(factor_indices);
        free(factor_exps);
        return (seq_t)1;
    }

    /* find maximum prime index */
    uint32_t max_idx = 0;
    for (size_t i = 0; i < fcnt; ++i)
        if (factor_indices[i] > max_idx)
            max_idx = factor_indices[i];

    /* allocate array of exponents per prime index (1‑based) */
    uint32_t *exp_by_idx = (uint32_t *)calloc(max_idx + 1, sizeof(uint32_t));
    if (!exp_by_idx) exit(1);

    for (size_t i = 0; i < fcnt; ++i)
        exp_by_idx[ factor_indices[i] ] = factor_exps[i];

    /* compute divisor count as product over k=1..max_idx of (E_k + 1)
       where E_k = sum_{j>=k} exp_by_idx[j] */
    seq_t result = 1;
    uint32_t running = 0;
    for (int k = (int)max_idx; k >= 1; --k)
    {
        running += exp_by_idx[(size_t)k];
        result *= (seq_t)(running + 1);
    }

    free(factor_indices);
    free(factor_exps);
    free(exp_by_idx);

    return result;
}
