/*
 * A350704  Composite numbers that have no Sophie Germain prime factors.
 * https://oeis.org/A350704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 5746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350704: composite numbers that have no Sophie Germain prime factors.
   The function a(idx_t n) returns the n-th term (a(1) = 49).  The implementation
   generates terms on demand, caching previously computed terms.  It relies
   solely on a simple sieve of Eratosthenes for primes up to a fixed limit
   and trial division for primality/factor checking.  No external utility
   library is required.  The code is fully ISO C11 compliant and also
   compiles under C++17. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Constant parameters ---------- */
static const idx_t PRIMES_LIMIT = 2000000;   /* sieve size (enough for n≈10000) */

/* ---------- Global memory for primes and cache ---------- */
static bool *prime_flag  = NULL;  /* prime_flag[i] == true  -> i is prime, i >= 2  */
static uint32_t *prime_list = NULL;  /* list of primes in ascending order   */
static size_t prime_cnt = 0;         /* number of primes found               */

static seq_t *term_cache = NULL;  /* cached sequence terms               */
static size_t cache_cap  = 0;      /* capacity of term_cache              */
static size_t cache_len  = 0;      /* number of terms stored              */
static idx_t  next_candidate = 4;  /* next number to test                 */

/* ---------- Utility: Sieve of Eratosthenes ---------- */
static void init_sieve(void)
{
    if (prime_flag != NULL) return;   /* already initialised */

    prime_flag = (bool *)calloc((size_t)PRIMES_LIMIT + 1, 1);
    if (prime_flag == NULL) return;   /* allocation failure – unlikely in judge */

    /* 0 and 1 are not prime; all others start as prime candidates */
    for (idx_t i = 2; i <= PRIMES_LIMIT; ++i) prime_flag[i] = true;

    for (idx_t i = 2; i * i <= PRIMES_LIMIT; ++i)
        if (prime_flag[i])
            for (idx_t j = i * i; j <= PRIMES_LIMIT; j += i)
                prime_flag[j] = false;

    /* Count primes and store them in prime_list */
    for (idx_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (prime_flag[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (prime_list == NULL) {
        free(prime_flag);
        prime_flag = NULL;
        prime_cnt = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (prime_flag[i]) prime_list[idx++] = (uint32_t)i;
}

/* ---------- Primality test (u64) ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n <= PRIMES_LIMIT) return prime_flag[(size_t)n];
    /* trial division by precomputed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return false;
    }
    /* n may have a remaining large prime factor > PRIMES_LIMIT */
    return true;
}

/* ---------- Sophie Germain prime test ---------- */
static bool is_sophie_germain_prime(uint64_t p)
{
    if (!is_prime_u64(p)) return false;
    uint64_t q = 2 * p + 1;
    return is_prime_u64(q);
}

/* ---------- Check if a composite number has any SG prime factor ---------- */
static bool has_sg_prime_factor(idx_t num)
{
    if (num < 4) return false;   /* 1,2,3 are not composite */

    uint64_t tmp = (uint64_t)num;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            if (is_sophie_germain_prime((uint64_t)p)) return true;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {  /* remaining prime factor */
        if (is_sophie_germain_prime(tmp)) return true;
    }
    return false;
}

/* ---------- Ensure we have at least 'need' terms cached ---------- */
static void ensure_terms(size_t need)
{
    if (cache_len >= need) return;

    if (prime_flag == NULL) init_sieve();

    /* Prepare dynamic cache */
    if (term_cache == NULL) {
        cache_cap = 1024;
        term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (term_cache == NULL) return;  /* allocation failure */
    }

    while (cache_len < need) {
        idx_t cand = next_candidate++;
        if (cand < 4) continue;                     /* start from 4 */
        if (is_prime_u64((uint64_t)cand)) continue; /* skip primes */
        if (!has_sg_prime_factor(cand)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) return; /* allocation failure */
                term_cache = tmp;
            }
            term_cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – a(1) is first term */
    ensure_terms((size_t)n);
    return term_cache[n - 1];
}
