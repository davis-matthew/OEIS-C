/*
 * A249436  Integers n such that n/2 < A249151(n) < n.
 * https://oeis.org/A249436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249436
   a(n) = the n-th integer k such that
          the product of all entries in row k of
          Pascal's triangle is divisible by ((k+1)/2)! but not by k! */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* --------- configuration ---------------------------------------------- */
#define PRIME_LIMIT 2000000   /* upper bound for k to be examined */
/* ------------------------------------------------------------------------ */

static uint32_t *primes = NULL;      /* array of primes up to PRIME_LIMIT      */
static size_t    prime_cnt = 0;       /* number of primes                       */
static bool      primes_ready = false;

/* ------------------------------------------------------------------------ */
/* simple Sieve of Eratosthenes to fill |primes, prime_cnt.                */
static void prepare_primes(void)
{
    if (primes_ready) return;
    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    for (uint32_t i = 2; i * (uint64_t)i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_cnt++;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    free(is_comp);
    primes_ready = true;
}

/* ------------------------------------------------------------------------ */
/* Sum_{i=1}^{n} floor(i/d)  –  closed form, works for n,d <= 64-bit     */
static __uint128_t sum_floor_div(uint64_t n, uint64_t d)
{
    /* n/d = m, r = n % d */
    uint64_t m = n / d;
    if (m == 0) return 0;
    /* part1 = d * m*(m-1)/2 */
    __uint128_t part1 = (__uint128_t)d * ( (__uint128_t)m * (m - 1) / 2 );
    /* part2 = m * (n - m*d + 1)   (n - m*d = r) */
    __uint128_t part2 = (__uint128_t)m * (n - m * d + 1);
    return part1 + part2;
}

/* ------------------------------------------------------------------------ */
/* Test whether a given k satisfies the definition of A249436.               */
static bool good_k(uint64_t k)
{
    uint64_t m = (k + 1) >> 1;   /* floor((k+1)/2) */

    bool seen_smaller = false;

    for (size_t pi = 0; pi < prime_cnt && primes[pi] <= k; ++pi)
    {
        uint64_t p = primes[pi];
        /* ---------- compute A = vp(k!) and B = sum_{i=1}^k vp(i!) -------- */
        __uint128_t A = 0, B = 0;
        for (uint64_t d = p; d <= k; d *= p)
        {
            uint64_t q = k / d;
            A += q;
            B += sum_floor_div(k, d);
            if (d > k / p) break;          /* avoid overflow of d*p      */
        }

        /* ---------- compute vp(m!) -------------------------------------- */
        __uint128_t vp_m = 0;
        for (uint64_t d = p; d <= m; d *= p)
        {
            vp_m += m / d;
            if (d > m / p) break;
        }

        /* ---------- exponent in the product of binomials ---------------- */
        __uint128_t e = (__uint128_t)(k + 1) * A - 2 * B;

        /* if not divisible by m!, reject this k */
        if (e < vp_m)
            return false;

        /* record if it is NOT divisible by k! */
        if (!seen_smaller && e < A)
            seen_smaller = true;
    }

    return seen_smaller;   /* admissible iff not divisible by k!            */
}

/* ------------------------------------------------------------------------ */
/* main sequence function -------------------------------------------------- */
static seq_t *seq_cache = NULL;   /* dynamic array of found values */
static size_t seq_len  = 0;       /* number of elements stored          */
static size_t seq_cap  = 0;       /* current capacity                   */
static uint64_t next_k  = 1;      /* next integer to test                */

seq_t a(idx_t n)
{
    /* initialise everything lazily on first call */
    if (!primes_ready)
        prepare_primes();

    /* grow the cache as necessary */
    while (seq_len < n)
    {
        if (next_k > PRIME_LIMIT)
            abort();   /* enough for all test cases according to the limits  */

        if (good_k(next_k))
        {
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_cache = (seq_t *)realloc(seq_cache,
                                              seq_cap * sizeof(seq_t));
                if (!seq_cache) abort();
            }
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
