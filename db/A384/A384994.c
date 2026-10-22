/*
 * A384994  Positive numbers whose prime indices of their prime factors,
 *          together with their exponents are distinct.
 * https://oeis.org/A384994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A384994 – Positive numbers whose prime indices of their prime
    factors, together with their exponents, are distinct.
    The sequence starts with 1,3,4,5,7,8,11,13,...  */

/* Standard headers (C11 / C++17) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Helper available in the build: number of primes ≤ n. */
#include "../utils/prime_pi.h"

#define SIEVE_MAX      2000000U          /* upper bound for trial division */
#define PRIME_LIST_CAP 250000U           /* enough for primes ≤ SIEVE_MAX */

static unsigned char *prime_is_prime = NULL; /* temporary sieve array */
static uint32_t prime_list[PRIME_LIST_CAP];   /* list of primes */
static size_t prime_cnt = 0;                  /* how many primes we found */

/*------------------------------------------------------------*/
/* Build a prime sieve once and store the primes in an array */
static void init_primes(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    prime_is_prime = (unsigned char *)malloc(SIEVE_MAX + 1);
    if (!prime_is_prime) exit(1);
    for (size_t i = 0; i <= SIEVE_MAX; ++i) 
        prime_is_prime[i] = 1;
    prime_is_prime[0] = prime_is_prime[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_MAX; ++i) {
        if (prime_is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_MAX; j += i)
                prime_is_prime[j] = 0;
        }
    }

    prime_cnt = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i) 
        if (prime_is_prime[i]) prime_list[prime_cnt++] = (uint32_t)i;

    free(prime_is_prime);
    prime_is_prime = NULL;
}

/*------------------------------------------------------------*/
/* Return the 1‑based index of prime p (p assumed to be prime) */
static idx_t prime_index(uint64_t p)
{
    if (p <= SIEVE_MAX) {
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if ((uint64_t)prime_list[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        return (idx_t)(lo + 1);           /* primes are 1‑based */
    } else {
        /* p is larger than the sieve – count via the helper */
        seq_t pi = prime_pi((idx_t)p);
        return (idx_t)pi;                 /* cast is safe for the ranges used */
    }
}

/*------------------------------------------------------------*/
/* Quick trial‑division factorisation (uses the pre‑computed primes) */
static void factor_uint64(uint64_t n,
                          uint64_t *p_arr, int *e_arr, size_t *cnt)
{
    size_t k = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            p_arr[k] = p;
            e_arr[k] = e;
            ++k;
        }
    }
    if (tmp > 1) {                 /* remaining factor is prime */
        p_arr[k] = tmp;
        e_arr[k] = 1;
        ++k;
    }
    *cnt = k;
}

/*------------------------------------------------------------*/
/* Test whether n is a term of the sequence */
static bool valid_candidate(uint64_t n)
{
    if (n == 1) return true;

    uint64_t primes[64];
    int exps[64];
    size_t nf = 0;
    factor_uint64(n, primes, exps, &nf);

    /* build a list of all indices and exponents */
    uint64_t dims[128];
    size_t vidx = 0;
    for (size_t i = 0; i < nf; ++i) {
        dims[vidx++] = (uint64_t)prime_index(primes[i]); /* prime index */
        dims[vidx++] = (uint64_t)exps[i];               /* exponent      */
    }

    /* check for duplicates */
    for (size_t i = 0; i < vidx; ++i) {
        for (size_t j = i + 1; j < vidx; ++j) {
            if (dims[i] == dims[j]) return false;
        }
    }
    return true;
}

/*------------------------------------------------------------*/
/* Memoised storage for generated terms */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/*------------------------------------------------------------*/
/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0 – return 0 */

    init_primes();                  /* ensure primes are ready */

    /* initialise storage on first call */
    if (!seq_buf) {
        seq_cap = 128;
        seq_buf = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq_buf) exit(1);
    }

    /* generate terms up to the required index */
    while (seq_len < (size_t)n) {
        /* grow buffer if needed */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_buf, sizeof(seq_t) * seq_cap);
            if (!tmp) exit(1);
            seq_buf = tmp;
        }

        uint64_t cand;
        if (seq_len == 0) cand = 1;
        else cand = (uint64_t)seq_buf[seq_len - 1] + 1;

        /* find the next valid number */
        while (!valid_candidate(cand))
            ++cand;

        seq_buf[seq_len++] = (seq_t)cand;
    }

    return seq_buf[n - 1];
}
