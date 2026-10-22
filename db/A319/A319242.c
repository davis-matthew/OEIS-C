/*
 * A319242  Heinz numbers of strict integer partitions of odd numbers.
 *          Squarefree numbers whose prime indices sum to an odd number.
 * https://oeis.org/A319242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th square‑free integer whose prime indices sum to
 *            an odd number. The program tests each integer in order, stores
 *            qualifying values in a cache, and returns the n‑th entry
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Prime list handling -------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* dynamic array of primes       */
static size_t   prime_cnt  = 0;       /* number of primes stored       */

/* Add a prime to the list, expanding the buffer if necessary. */
static void append_prime(uint64_t p)
{
    static size_t prime_cap = 0;
    if (prime_cnt >= prime_cap) {
        prime_cap = (prime_cap == 0 ? 128 : prime_cap * 2);
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_cnt++] = p;
}

/* Ensure that all primes up to 'limit' inclusively are in the list. */
static void ensure_primes_upto(uint64_t limit)
{
    if (!prime_list) {            /* initialise with first prime */
        append_prime(2);
    }
    if (prime_list[prime_cnt - 1] >= limit) return;

    uint64_t cand = prime_list[prime_cnt - 1] + 1;
    if (cand <= 2) cand = 3;
    if ((cand & 1ULL) == 0) cand++;   /* start with odd if >2 */

    for (; cand <= limit; cand += 2) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= cand / prime_list[i]; ++i) {
            if (cand % prime_list[i] == 0ULL) { is_prime = false; break; }
        }
        if (is_prime) append_prime(cand);
    }
}

/* Return the 0‑based index of prime 'p' in prime_list; assumes p is prime. */
static size_t prime_index_of(uint64_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] == p) return i;
    }
    /* Should not happen if caller ensures p is in the list. */
    return prime_cnt;
}

/* --------------------------------------------------------------------- */
/* Integer square root for uint64_t ------------------------------------ */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;   /* highest power of four <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* --------------------------------------------------------------------- */
/* Predicate: true if num is square‑free and the sum of prime indices is odd. */
static bool passes(uint64_t num)
{
    if (num < 2ULL) return false;

    uint64_t tmp = num;
    int      sum = 0;                     /* 1‑based indices */

    uint64_t sqrt_tmp = isqrt_u64(tmp);
    ensure_primes_upto(sqrt_tmp);

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_tmp; ++i) {
        uint64_t p = prime_list[i];
        int      exp = 0;
        while (tmp % p == 0ULL) {
            tmp /= p;
            ++exp;
            if (exp > 1) return false;   /* not square‑free */
        }
        if (exp == 1) sum += (int)(i + 1);   /* 1‑based index */
    }

    if (tmp > 1ULL) {   /* remaining factor is prime > sqrt(num) */
        ensure_primes_upto(tmp);
        size_t idx = prime_index_of(tmp);
        sum += (int)(idx + 1);
    }

    return (sum & 1) != 0;   /* odd sum required */
}

/* --------------------------------------------------------------------- */
/* Sequence cache ------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of sequence values */
static idx_t  cache_size = 0;      /* number of cached entries   */
static size_t cache_cap  = 0;      /* allocated capacity          */

/* Ensure cache can hold at least 'n' entries. */
static void ensure_cache_capacity(idx_t n)
{
    if ((idx_t)cache_cap >= n) return;
    size_t new_cap = (size_t)(n * 3 / 2 + 10);
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Global counter of the next integer to test. */
static uint64_t next_check = 2ULL;

/* --------------------------------------------------------------------- */
/* Sequence factory ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;                 /* undefined for n=0          */

    if (!cache) {                          /* initial allocation        */
        cache_cap   = 128;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size  = 0;
    }

    ensure_cache_capacity(n);

    while (cache_size < n) {
        if (passes(next_check)) {
            cache[cache_size++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return cache[n - 1];
}
