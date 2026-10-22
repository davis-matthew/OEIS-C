/*
 * A365865  Starts of runs of 3 consecutive integers that are divisible by the
 *          square of their least prime factor.
 * https://oeis.org/A365865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the start of each run of three consecutive integers that are
   terms of A283050, i.e., each integer is divisible by the square of its
   least prime factor.  The function is memoised so it works correctly for
   arbitrary order of calls and any index n ≥ 1.  It uses a simple
   incremental prime sieve and tests each candidate integer.  All dynamic
   allocations are cast explicitly so the code compiles as both C and C++. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve ------------------------------------------ */
static uint64_t *prime_vals = (uint64_t*)NULL;   /* array of primes          */
static size_t   prime_cnt  = 0;                  /* number of primes found   */
static size_t   prime_cap  = 0;                  /* capacity of prime_vals   */
static size_t   sieve_limit = 0;                /* maximum number indexed   */

static uint64_t int_sqrt_u64(uint64_t n)
{
#if defined(_floor_isqrt_u64)
    return _floor_isqrt_u64((idx_t)n);
#else
    uint64_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else        high = mid - 1;
    }
    return high;
#endif
}

static void ensure_primes(uint64_t need)
{
    if (need <= sieve_limit) return;

    size_t new_limit = sieve_limit ? sieve_limit : 128;
    while (new_limit < need) new_limit <<= 1;

    bool *sieve = (bool*)calloc(new_limit + 1, 1);
    if (!sieve) abort();

    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                sieve[j] = 1;
        }
    }

    size_t new_cap = new_limit / 10 + 64;
    uint64_t *tmp = (uint64_t*)realloc(prime_vals, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_vals = tmp;
    prime_cap  = new_cap;
    prime_cnt  = 0;

    for (size_t i = 2; i <= new_limit; ++i)
        if (!sieve[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                uint64_t *t2 = (uint64_t*)realloc(prime_vals, prime_cap * sizeof(uint64_t));
                if (!t2) abort();
                prime_vals = t2;
            }
            prime_vals[prime_cnt++] = i;
        }

    free(sieve);
    sieve_limit = new_limit;
}

static uint64_t least_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t limit = int_sqrt_u64(n);
    ensure_primes(limit);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_vals[i];
        if (p > limit) break;
        if (n % p == 0) return p;
    }
    return n;   /* n is prime */
}

/* ---------- Sequence generation ---------------------------------- */
static seq_t *term_cache = (seq_t*)NULL;
static size_t term_len   = 0;
static size_t term_cap   = 0;
static uint64_t candidate = 2;   /* next integer to test as run start */

static void grow_terms(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = term_cap ? term_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    term_cache = tmp;
    term_cap   = new_cap;
}

static bool is_a283050(uint64_t m)
{
    if (m < 2) return false;
    uint64_t p = least_prime_factor(m);
    uint64_t sq = p * p;
    return (m % sq) == 0;
}

/* ---------- Public function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t idx = (size_t)n;
    if (idx > term_len) {
        grow_terms(idx);
        while (term_len < idx) {
            if (is_a283050(candidate) &&
                is_a283050(candidate + 1) &&
                is_a283050(candidate + 2)) {
                term_cache[term_len++] = (seq_t)candidate;
            }
            ++candidate;
        }
    }
    return term_cache[idx - 1];
}
