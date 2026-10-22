/*
 * A341321  Composite numbers k that are divisible by (k mod
 *          sopfr(k))+sopfr(k), where sopfr = A001414.
 * https://oeis.org/A341321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5533, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search the sequence of composite numbers k that satisfy
   k % ((k % sopfr(k)) + sopfr(k)) == 0.
   a(1) = 4, a(2) = 6, a(3) = 8, …  (see OEIS A341321) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: integer square root of a 64‑bit unsigned value */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 1ULL << 32;          /* high^2 > max 64‑bit */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)  /* mid*mid > n ? */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Prime sieve – dynamic list of primes */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;
static uint64_t prime_limit = 0;   /* largest number that has been sieved */

static void grow_primes(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;
    for (uint64_t num = (prime_limit < 2 ? 2 : prime_limit + 1);
         num <= limit; ++num) {
        bool is_p = true;
        uint64_t r = isqrt_u64(num);
        for (size_t i = 0; i < primes_cnt && primes[i] <= r; ++i) {
            if (num % primes[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = num;
        }
    }
    prime_limit = limit;
}

static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    uint64_t r = isqrt_u64(n);
    generate_primes_up_to(r);
    for (size_t i = 0; i < primes_cnt && primes[i] <= r; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* Sum of prime factors of n with multiplicity (sopfr) */
static seq_t sopfr_uint64(uint64_t n)
{
    seq_t sum = 0;
    if (n < 2) return sum;
    uint64_t r = isqrt_u64(n);
    generate_primes_up_to(r);
    for (size_t i = 0; i < primes_cnt && primes[i] <= r; ++i) {
        uint64_t p = primes[i];
        while (n % p == 0) {
            sum += (seq_t)p;
            n /= p;
            r = isqrt_u64(n);
        }
    }
    if (n > 1) sum += (seq_t)n;
    return sum;
}

/* Cache of produced sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static uint64_t current_k = 2;   /* next integer to test */

/* Ensure the cache has capacity for at least 'needed' values */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void add_to_cache(seq_t val)
{
    ensure_cache(cache_size + 1);
    cache[cache_size++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1, follow convention */
    if (n <= cache_size) return cache[n-1];

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        ++current_k;
        if (is_prime_small(current_k)) continue;          /* skip primes */
        seq_t sop = sopfr_uint64(current_k);
        seq_t rem = (seq_t)current_k % sop;
        seq_t add = rem + sop;
        if (add > 0 && (seq_t)current_k % add == 0)
            add_to_cache((seq_t)current_k);
    }
    return cache[n-1];
}
