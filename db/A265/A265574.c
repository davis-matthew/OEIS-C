/*
 * A265574  LCM-transform of triangular numbers.
 * https://oeis.org/A265574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of prime powers in t_n that were not present in
 *            any earlier t_k with k < n, where t_n = n*(n+1)/2. In other
 *            words, a(n) = t_n / gcd(t_n, LCM(t_1,…,t_{n-1})). a(1) = 1; all
 *            other terms follow from the recurrence above
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint32_t, uint64_t */
#include <stdbool.h>  /* bool */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* globals ------------------------------------------------------------*/

static seq_t *a_cache = NULL;             /* a values */
static size_t a_cache_cap = 0;            /* allocated size */
static idx_t  last_computed = 0;          /* highest n computed so far */

#define PRIME_LIMIT 10001                /* n(n+1)/2 for n ≤ 10000 */
static int primes[1300];                 /* list of primes ≤ PRIME_LIMIT */
static int prime_cnt = 0;
static int prime_index[PRIME_LIMIT + 1];  /* prime → index in primes[] */
static bool primes_ready = false;
static int max_exp[1300] = {0};           /* max exponent seen so far for each prime */

/* helper ------------------------------------------------------------*/

/* sieve to generate primes up to PRIME_LIMIT and initialise prime_index */
static void init_primes(void)
{
    if (primes_ready) return;
    bool is_prime[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    prime_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;

    for (int i = 0; i <= PRIME_LIMIT; ++i) prime_index[i] = -1;
    for (int i = 0; i < prime_cnt; ++i) prime_index[primes[i]] = i;

    primes_ready = true;
}

/* compute a(n) ------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – behaviour irrelevant */

    /* initialise primes once */
    if (!primes_ready) init_primes();

    /* allocate or grow cache */
    if (a_cache == NULL) {
        a_cache_cap = (size_t)n + 1;
        a_cache = seq_alloc(a_cache_cap);
    } else if ((size_t)n + 1 > a_cache_cap) {
        a_cache_cap = (size_t)n + 1;
        a_cache = seq_realloc(a_cache, a_cache_cap, a_cache_cap);
    }

    /* compute missing values up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        uint64_t kk = (uint64_t)k;
        uint64_t tn = kk * (kk + 1) / 2;       /* triangular number */
        uint64_t temp = tn;
        seq_t a_val = 1;

        /* factor t_n using the pre‑computed primes */
        for (int pi = 0; pi < prime_cnt &&
                         (uint64_t)primes[pi] * (uint64_t)primes[pi] <= temp;
             ++pi) {
            uint32_t p = primes[pi];
            unsigned cnt = 0;
            while (temp % p == 0) {
                ++cnt;
                temp /= p;
            }
            if (cnt) {
                unsigned prev = max_exp[pi];
                if (cnt > prev) {
                    unsigned delta = cnt - prev;
                    seq_t pow = 1;
                    for (unsigned d = 0; d < delta; ++d) pow *= (seq_t)p;
                    a_val *= pow;
                    max_exp[pi] = cnt;
                }
            }
        }

        /* remaining prime factor, if any */
        if (temp > 1) {
            uint32_t p = (uint32_t)temp;
            int pi = prime_index[p];          /* p ≤ PRIME_LIMIT */
            if (pi >= 0) {
                unsigned prev = max_exp[pi];
                if ((unsigned)1 > prev) {
                    a_val *= (seq_t)p;
                    max_exp[pi] = 1;
                }
            }
        }

        a_cache[(size_t)k] = a_val;
    }

    last_computed = n;
    return a_cache[(size_t)n];
}
