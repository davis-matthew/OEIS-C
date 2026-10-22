/*
 * A359905  Numbers whose prime indices and prime signature both have integer
 *          mean.
 * https://oeis.org/A359905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A359905: numbers whose prime indices and prime signature both have integer mean. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- global data ---------- */
static uint32_t *primes = NULL;          /* array of primes, 1‑based 1st prime = 2 */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static uint32_t *prime_index = NULL;     /* prime_index[p] = index of prime p (1‑based) */
static uint64_t sieve_limit = 0;        /* current max prime considered */

static seq_t *cache = NULL;             /* sequence terms */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ---------- helpers ---------- */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* builds a prime sieve up to 'limit' (inclusive) and fills primes[] and prime_index[] */
static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;               /* already built up to this limit */

    /* free old data */
    free(primes);
    free(prime_index);

    /* allocate new arrays */
    prime_cap = limit / 10 + 10;          /* rough upper bound for number of primes */
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt = 0;

    prime_index = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));

    /* simple Eratosthenes */
    bool *is_prime = (bool*)malloc((size_t)limit + 1);
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) { /* resize if needed */
                prime_cap *= 2;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
            prime_index[i]   = (uint32_t)(prime_cnt);   /* 1‑based index */
        }
    }

    free(is_prime);
    sieve_limit = limit;
}

/* test if n has integer mean of prime indices and integer mean of exponents */
static bool qualifies(uint64_t n)
{
    uint64_t tmp = n;
    int      k   = 0;           /* number of distinct primes */
    uint64_t sum_exp = 0;
    __int128 sum_idx = 0;       /* sum of (exponent * prime_index) */

    /* ensure sieve large enough for any prime factor */
    if (tmp > sieve_limit) {
        /* extend sieve to at least the square root of the remaining value plus some margin */
        uint64_t new_limit = tmp > sieve_limit * 2 ? tmp : sieve_limit * 2;
        build_sieve(new_limit);
    }

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            ++k;
            sum_exp += e;
            sum_idx += (__int128)e * prime_index[p];
        }
    }

    if (tmp > 1) {   /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        if (p > sieve_limit) {  /* extend sieve to get its index */
            build_sieve(p);
        }
        ++k;
        sum_exp += 1;
        sum_idx += prime_index[p];
    }

    if (k == 0) return false;            /* n < 2 */
    if (sum_exp % k != 0) return false;   /* mean of exponents not integer */
    if (sum_idx % sum_exp != 0) return false; /* mean of indices not integer */
    return true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t candidate = 2;

    if (!initialized) {
        /* start with a reasonable sieve size */
        build_sieve(1000000);              /* 1 million works for n up to many thousands */
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        if (qualifies(candidate)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
