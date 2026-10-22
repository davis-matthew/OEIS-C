/*
 * A331995  Numbers with at most one distinct prime prime index.
 * https://oeis.org/A331995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A331995: numbers that have at most one distinct prime prime index.
   A prime index of n is a number m such that prime(m) divides n.
   The set of prime indices of n is considered, and the term is accepted if
   at most one of those indices is itself a prime number. */

/* Standard headers ---------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- adjustable constants ---------- */
#define INITIAL_LIMIT 200000   /* pre‑sieved limit (enough for n up to ~10000) */

/* ---------- static data structures ---------- */
static bool   initialized = false;
static size_t limit;                 /* sieve limit */

static uint64_t *spf;                /* smallest prime factor for each number */
static uint32_t *prime_idx;          /* prime_idx[p] = position of prime p (1‑based) */
static bool     *prime_idx_is_prime;/* true if prime_idx[p] is a prime number */
static uint32_t *primes;             /* list of primes up to limit */
static size_t   prime_cnt;           /* number of primes */

static seq_t   *terms;    /* cached sequence values */
static size_t   terms_cap = 0;
static size_t   terms_cnt = 0;
static idx_t    next_candidate = 1;  /* next number to test */

/* ---------- helper functions ---------- */
static void grow_array(void **ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need)
    {
        size_t newcap = *cap ? *cap * 2 : 8;
        while (newcap < need) newcap *= 2;
        *ptr = realloc(*ptr, newcap * elem_size);
        *cap = newcap;
    }
}

/* Count distinct prime factors of n whose index is a prime */
static void factor_and_check(idx_t n, size_t *cnt)
{
    *cnt = 0;
    if (n == 1) return;

    idx_t x = n;
    while (x > 1)
    {
        uint32_t p = spf[x];
        if (prime_idx_is_prime[ prime_idx[p] ]) (*cnt)++;
        while (x % p == 0) x /= p;
        if (*cnt > 1) break; /* early exit */
    }
}

static void init_precompute(void)
{
    limit = INITIAL_LIMIT;

    spf                = (uint64_t *) calloc(limit + 1, sizeof(uint64_t));
    prime_idx          = (uint32_t *) calloc(limit + 1, sizeof(uint32_t));
    primes             = (uint32_t *) malloc(sizeof(uint32_t) * (limit / 10 + 1));
    prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)            /* i is prime */
        {
            spf[i] = i;
            prime_idx[i] = ++prime_cnt;
            primes[prime_cnt - 1] = (uint32_t)i;

            if (i <= limit / i)     /* avoid overflow */
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    primes = (uint32_t *) realloc(primes, prime_cnt * sizeof(uint32_t));

    prime_idx_is_prime = (bool *) calloc(prime_cnt + 1, sizeof(bool));
    for (size_t i = 2; i <= prime_cnt; ++i)   /* 0 and 1 are not prime indices */
    {
        bool is_pr = true;
        for (size_t j = 2; j * j <= i; ++j)
        {
            if (i % j == 0) { is_pr = false; break; }
        }
        prime_idx_is_prime[i] = is_pr;
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) { init_precompute(); initialized = true; }

    while (terms_cnt < n)
    {
        idx_t val = next_candidate++;
        bool ok = true;
        if (val > limit)
        {
            /* In the unlikely event our pre‑computed limit is too small
               we stop here – for n ≤ 10000 the initial limit is sufficient. */
            ok = false;
        }
        else
        {
            size_t cnt;
            factor_and_check(val, &cnt);
            if (cnt > 1) ok = false;
        }
        if (ok)
        {
            grow_array((void **)&terms, &terms_cap, terms_cnt + 1, sizeof(seq_t));
            terms[terms_cnt++] = (seq_t)val;
        }
    }

    return terms[n - 1];
}
