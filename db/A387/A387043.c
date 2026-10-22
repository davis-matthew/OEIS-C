/*
 * A387043  Number of ways to write n as (1 + (n mod 2))*p + prime(k) +
 *          prime(k+1) + prime(k+2), where p is a prime and k is a positive
 *          integer.
 * https://oeis.org/A387043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as (1 + (n mod 2))*p + prime(k)
 *            + prime(k+1) + prime(k+2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by
   (1) generating all primes up to n with a simple sieve,
   (2) for each n counting all triples of consecutive primes whose sum
       together with an admissible prime p equals n,
   (3) caching the result for future calls.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Sieve of Eratosthenes (simple, 1‑byte per number) ---------- */
static uint8_t *sieve = NULL;          /* 0 = prime, 1 = composite */
static uint32_t *prime_list = NULL;    /* primes in ascending order */
static size_t prime_cnt = 0;           /* number of primes in prime_list */
static idx_t sieve_limit = 0;          /* current sieve coverage */

static void ensure_sieve(idx_t limit)
{
    if (sieve_limit >= limit) return;  /* already large enough */

    /* free old data */
    free(sieve);
    free(prime_list);

    size_t sz = limit + 1;
    sieve = (uint8_t*)calloc(sz, 1);   /* 0 = prime */
    if (!sieve) return;
    sieve[0] = sieve[1] = 1;           /* 0 and 1 are not prime */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* collect primes */
    prime_cnt = 0;
    size_t cap = limit / 10 + 10;       /* generous initial capacity */
    prime_list = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!prime_list) return;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_cnt >= cap) {     /* grow if needed */
                cap *= 2;
                prime_list = (uint32_t*)realloc(prime_list,
                                                cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }
    sieve_limit = limit;
}

/* simple primality check using the sieve (valid only for numbers ≤ sieve_limit) */
static inline bool prime_sieve(idx_t x)
{
    return x >= 2 && x <= sieve_limit && !sieve[x];
}

/* ---------- Count representations for a single n ---------- */
static uint64_t count_representations(idx_t n)
{
    uint64_t cnt = 0;

    /* smallest possible sum is 10 (2+3+5) + 2 (for even) or 2*2=4 (for odd) */
    if (n < 10) return 0;

    bool is_even = ((n & 1) == 0);
    idx_t max_sum = is_even ? n - 2 : n - 4;

    /* iterate over triples of consecutive primes */
    for (size_t i = 0; i + 2 < prime_cnt; ++i) {
        idx_t sum3 = (idx_t)prime_list[i] + prime_list[i+1] + prime_list[i+2];
        if (sum3 > max_sum) break;

        idx_t rest = n - sum3;
        if (is_even) {
            if (prime_sieve(rest))
                ++cnt;
        } else {
            if ((rest & 1) != 0) continue;   /* n - sum3 must be even */
            idx_t p = rest >> 1;
            if (prime_sieve(p))
                ++cnt;
        }
    }
    return cnt;
}

/* ---------- Cache for previously computed a(n) ---------- */
static uint64_t *int_cache = NULL;     /* cached values, 1‑indexed */
static idx_t cache_max = 0;           /* highest n with cached value */

/* ---------- Public function required by the specification ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* domain starts at 1 */

    /* Ensure sieve and cache are large enough */
    if (n > sieve_limit)                     /* generate primes up to n */
        ensure_sieve(n);

    if (n > cache_max) {
        /* allocate or grow integer cache */
        size_t new_size = n + 1;             /* 1‑indexed */
        uint64_t *new_cache = (uint64_t*)calloc(new_size, sizeof(uint64_t));
        if (!new_cache) return 0;            /* allocation failure */

        if (int_cache) {
            memcpy(new_cache, int_cache,
                   (size_t)(cache_max + 1) * sizeof(uint64_t));
            free(int_cache);
        }

        /* compute missing values */
        for (idx_t i = cache_max + 1; i <= n; ++i)
            new_cache[i] = count_representations(i);

        int_cache = new_cache;
        cache_max = n;
    }

    return (seq_t)int_cache[n];
}
