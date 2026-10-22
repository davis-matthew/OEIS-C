/*
 * A324923  Number of distinct factors in the factorization of n into factors
 *          q(i) = prime(i)/i, i > 0.
 * https://oeis.org/A324923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of distinct factors in the factorization of n into
 *            factors q(i) = prime(i)/i, i>0. This implementation computes the
 *            unique representation of n in terms of the q(i) and counts how
 *            many distinct indices i occur. It works for any n ≥ 1 and uses
 *            only integer arithmetic
 * Range:     u8 n <= 8771, u16 n <= 8771, u32 n <= 8771, u64 n <= 8771,
 *            u128 n <= 8771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define LIMIT 20000            /* Upper bound for prime sieve */
#define MAX_PRIMES 3000        /* Enough for all primes ≤ LIMIT */

/* Global tables populated once */
static uint32_t primes[MAX_PRIMES];
static int      primes_cnt = 0;
static uint32_t spf[LIMIT + 1];     /* Smallest prime factor */
static uint32_t prime_index[LIMIT + 1]; /* 0‑based index of prime, 0 if composite */

/* One‑time sieve to fill primes[], spf[], and prime_index[] */
static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= LIMIT; ++i) spf[i] = 0;
    for (i = 0; i <= LIMIT; ++i) prime_index[i] = 0;

    for (i = 2; i <= LIMIT; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            prime_index[i] = primes_cnt;     /* index of this prime */
            primes[primes_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= LIMIT)
            {
                for (j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

static int initialized = 0;

seq_t a(idx_t n)
{
    /* Initialize tables on first call */
    if (!initialized)
    {
        init_sieve();
        initialized = 1;
    }

    uint64_t nn = (uint64_t)n;
    /* Count of prime exponents in n (index of primes) */
    int pcnt[MAX_PRIMES] = {0};

    /* Factorize n */
    while (nn > 1)
    {
        uint32_t p = spf[nn];
        int        id = prime_index[p];          /* 0‑based index of this prime */
        int        e  = 0;
        while (nn % p == 0)
        {
            nn /= p;
            ++e;
        }
        pcnt[id] += e;
    }

    seq_t ecnt[MAX_PRIMES + 2] = {0};  /* exponents of q(i); indices start at 1 */

    /* Process primes from largest to smallest */
    while (1)
    {
        int idx_p = -1;
        for (int i = primes_cnt - 1; i >= 0; --i)
            if (pcnt[i] > 0)
            {
                idx_p = i;
                break;
            }
        if (idx_p == -1) break;          /* all counts zero */

        int exp = pcnt[idx_p];
        pcnt[idx_p] = 0;
        int idx = idx_p + 1;             /* index i in q(i) */
        ecnt[idx] += exp;

        /* Update prime counts for denominator contribution of i */
        int val = idx;
        while (val > 1)
        {
            uint32_t p = spf[val];
            int cnt = 0;
            while (val % p == 0)
            {
                val /= p;
                ++cnt;
            }
            int pi = prime_index[p];     /* 0‑based index of prime p */
            pcnt[pi] += exp * cnt;
        }
    }

    /* Count distinct indices with positive exponent */
    seq_t result = 0;
    for (int i = 1; i <= primes_cnt + 1; ++i)
        if (ecnt[i] > 0) ++result;

    return result;
}
