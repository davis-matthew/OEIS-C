/*
 * A326844  Let y be the integer partition with Heinz number n. Then a(n) is
 *          the size of the complement, in the minimal rectangular partition
 *          containing the Young diagram of y, of the Young diagram of y.
 * https://oeis.org/A326844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_prime_index(n) * (sum of exponents in n) - (sum over
 *            primes p of (index_of(p) * exponent_of(p) in n)). Here
 *            index_of(p) is the 1‑based position of p in the sequence of
 *            primes. This is equivalent to the OEIS definition a(n) =
 *            A001222(n) * A061395(n) - A056239(n) when the partitions are...
 * Range:     u8 n <= 2907, u16 n <= 9929, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime table ------------------------------------------------ */
#define PRIME_SIEVE_LIMIT 200000U   /* > sqrt(10^12), ample for the tests */

static uint64_t *prime_list = NULL;  /* sorted array of primes */
static size_t   prime_count = 0;     /* number of primes in prime_list */

/* initialise prime_list on first use */
static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already initialised */

    /* sieve of Eratosthenes */
    size_t limit = PRIME_SIEVE_LIMIT;
    bool *mark = (bool *)calloc(limit + 1, sizeof(bool)); /* 0 = prime */
    if (!mark) return;                        /* allocation failed - unlikely */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!mark[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                mark[j] = true;
        }
    }

    /* temporary buffer for primes (estimated size) */
    size_t est = limit / 10; /* rough upper bound: lim / ln(lim) */
    uint64_t *tmp = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!tmp) { free(mark); return; }

    for (size_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            if (prime_count == est) { /* grow buffer if needed */
                est <<= 1;
                uint64_t *newtmp = (uint64_t *)realloc(tmp, est * sizeof(uint64_t));
                if (!newtmp) { free(tmp); free(mark); return; }
                tmp = newtmp;
            }
            tmp[prime_count++] = i;
        }
    }

    prime_list = tmp;
    free(mark);
}

/* binary search: return 1‑based index of prime p in prime_list, or 0 if p not found */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = prime_list[mid];
        if (v == p) return mid + 1;
        if (v < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* factorisation of n: fill arrays primes[] and exps[], set cnt.
   The arrays must be large enough (at most 64 factors for a 64‑bit n). */
static void factorise(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {                    /* remaining prime factor > sqrt(original n) */
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* ---------- sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();                /* ensure prime table exists */

    if (n == 1) return (seq_t)0;   /* empty partition */

    uint64_t pf[64];
    uint64_t pe[64];
    size_t   pcnt = 0;

    factorise((uint64_t)n, pf, pe, &pcnt);

    seq_t total_parts = 0;         /* S = sum of exponents */
    seq_t max_idx     = 0;         /* max prime index */
    seq_t sum_parts   = 0;         /* Σ index(p) * exp(p) */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t exp = pe[i];
        size_t   idx = prime_index(pf[i]);   /* 1‑based index; guaranteed non‑zero */
        total_parts += (seq_t)exp;
        if (idx > max_idx) max_idx = (seq_t)idx;
        sum_parts += (seq_t)idx * (seq_t)exp;
    }

    /* a(n) = max_idx * total_parts - sum_parts */
    return max_idx * total_parts - sum_parts;
}
