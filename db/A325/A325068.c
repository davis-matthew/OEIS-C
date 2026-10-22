/*
 * A325068  Prime numbers congruent to 1 modulo 16 representable neither by
 *          x^2 + 32*y^2 nor by x^2 + 64*y^2.
 * https://oeis.org/A325068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325068: Prime numbers ≡ 1 (mod 16) that are not representable
   as x² + 32 y² nor as x² + 64 y².
   The implementation enumerates primes ≡ 1 (mod 16) on demand,
   tests both representations by brute‑force search
   (which is fast enough for the ranges needed),
   and caches the results so repeated calls are O(1). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------- Integer square root for 64‑bit values (floor) --------- */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1) << 32;   /* high enough for any 64‑bit n */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* --------- Test whether p can be written as x² + D y² --------- */
static bool representable(uint64_t p, uint64_t D)
{
    uint64_t y_max = isqrt64(p / D);
    for (uint64_t y = 1; y <= y_max; ++y) {
        uint64_t rem = p - D * y * y;
        uint64_t x   = isqrt64(rem);
        if (x * x == rem)
            return true;
    }
    return false;
}

/* --------- Prime list (trial division, incremental) --------- */
static uint64_t *prime_list = NULL;   /* primes discovered so far           */
static size_t      prime_cnt  = 0;     /* number of primes in prime_list    */
static size_t      prime_cap  = 0;     /* capacity of prime_list            */
static uint64_t    last_checked = 3;   /* last odd number tested            */

static void init_primes(void)
{
    if (prime_cnt == 0) {
        prime_cap = 4;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;             /* start with 2 */
        prime_cnt = 1;
        last_checked = 3;
    }
}

static void grow_prime_list(void)
{
    prime_cap *= 2;
    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
}

static bool is_prime(uint64_t m)
{
    /* 0 and 1 are not primes */
    if (m < 2)
        return false;
    uint64_t limit = isqrt64(m);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i)
        if (m % prime_list[i] == 0)
            return false;
    return true;
}

static void ensure_primes_upto(uint64_t n)
{
    if (prime_cnt == 0)
        init_primes();

    /* ensure prime list contains all primes up to at least n */
    while (prime_cnt > 0 && prime_list[prime_cnt - 1] >= n)
        return;

    /* extend prime list up to at least n */
    while (last_checked <= n) {
        if (is_prime(last_checked)) {
            if (prime_cnt == prime_cap)
                grow_prime_list();
            prime_list[prime_cnt++] = last_checked;
        }
        last_checked += 2;            /* odd numbers only */
    }
}

/* --------- Cache for sequence values --------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of stored sequence terms   */
static size_t cache_cap  = 0;   /* allocated capacity               */

static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* --------- Next candidate (≡1 (mod 16)) to test --------- */
static uint64_t next_candidate = 17;

/* --------- Main sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1; a(0) is undefined but return 0 */
        return (seq_t)0;

    /* Ensure primes list is initialized */
    if (prime_cnt == 0)
        init_primes();

    /* Allocate cache if necessary */
    if (cache_size == 0)
        grow_cache();

    /* Generate additional terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        uint64_t sqrt_cand = isqrt64(cand);
        ensure_primes_upto(sqrt_cand);

        if (is_prime(cand)) {
            if (!representable(cand, 32) && !representable(cand, 64)) {
                if (cache_size == cache_cap)
                    grow_cache();
                cache[cache_size++] = (seq_t)cand;
            }
        }
        next_candidate += 16;          /* next number ≡ 1 (mod 16)          */
    }
    return cache[n - 1];
}
