/*
 * A365869  Numbers whose exponent of least prime factor in their prime
 *          factorization is even.
 * https://oeis.org/A365869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365869: numbers whose exponent of least prime factor is even.
   Formula: a(n) = the n‑th positive integer k such that the exponent of the
   least prime factor of k is a positive even number (2,4,6,…). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root of a 64‑bit number, using 128‑bit for safety */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (n < ((uint64_t)1 << 32)) ? n : ((uint64_t)1 << 32) - 1;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Dynamically expand the prime list up to a given limit. */
typedef struct {
    uint64_t *primes;
    size_t   cnt;
    size_t   cap;
    uint64_t max;          /* largest number for which primes are known */
} prime_list_t;

static prime_list_t prime_lst = {NULL, 0, 0, 0};

static void ensure_primes_leq(uint64_t limit)
{
    if (limit <= prime_lst.max) return;

    size_t need_cap = prime_lst.cap ? prime_lst.cap : 16;
    if (need_cap < limit / 10 + 16)
        need_cap = limit / 10 + 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_lst.primes, need_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_lst.primes = tmp;
    prime_lst.cap = need_cap;

    for (uint64_t cand = prime_lst.max + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;                /* skip 0 and 1 */
        uint64_t sq = isqrt_u64(cand);
        bool ok = true;
        for (size_t i = 0; i < prime_lst.cnt && prime_lst.primes[i] <= sq; ++i) {
            if (cand % prime_lst.primes[i] == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            if (prime_lst.cnt == prime_lst.cap) {
                size_t new_cap = prime_lst.cap * 2;
                uint64_t *t = (uint64_t *)realloc(prime_lst.primes, new_cap * sizeof(uint64_t));
                if (!t) abort();
                prime_lst.primes = t;
                prime_lst.cap = new_cap;
            }
            prime_lst.primes[prime_lst.cnt++] = cand;
        }
    }
    prime_lst.max = limit;
}

/* Smallest prime factor of n (returns n if n is prime). */
static inline uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;          /* should not be called for 0 or 1 */
    uint64_t lim = isqrt_u64(n);
    ensure_primes_leq(lim);
    for (size_t i = 0; i < prime_lst.cnt && prime_lst.primes[i] <= lim; ++i) {
        uint64_t p = prime_lst.primes[i];
        if (n % p == 0)
            return p;
    }
    return n;
}

/* True if the exponent of the least prime factor of n is even. */
static inline bool lpf_exponent_even(uint64_t n)
{
    uint64_t p = smallest_prime_factor(n);
    int exp = 0;
    while (n % p == 0) {
        n /= p;
        ++exp;
    }
    return (exp & 1) == 0;
}

/* Sequence cache ----------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t current  = 1;   /* integer to test; starts at 2 on first increment */

static void init_sequence(void)
{
    if (cache) return;
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache_size = 0;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1, but safe default */

    init_sequence();

    while (cache_size < (size_t)n) {
        ++current;                  /* test next integer */
        if (lpf_exponent_even(current)) {   /* property holds */
            if (cache_size == cache_cap) {  /* grow cache */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_size++] = (seq_t)current;
        }
    }
    return cache[n - 1];
}
