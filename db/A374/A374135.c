/*
 * A374135  Nonmultiples of 3 whose sum of divisors is also a nonmultiple of
 *          3.
 * https://oeis.org/A374135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 8331, u32 n <= 9280, u64 n <= 9280,
 *            u128 n <= 9280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A374135 – Nonmultiples of 3 whose sum of divisors is also a nonmultiple of 3
 *  This implementation builds the sequence on demand by iterating over all
 *  positive integers, filtering those not divisible by 3 and whose σ(n) mod 3
 *  ≠ 0.  The σ(n) (sum of divisors) is computed modulo 3 by factorising n
 *  and multiplying the geometric sums for each prime power, all performed
 *  modulo 3 to keep values small.
 *
 *  The function keeps a static cache of found terms and a static list of
 *  primes up to the square root of the largest number examined.  The cache
 *  is grown as needed; subsequent calls can seek arbitrary n without
 *  re-computation.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *seq_cache = NULL;          /* cache of terms a(k) */
static size_t cache_size = 0;            /* number of cached terms          */
static size_t cache_cap = 0;             /* current allocation capacity     */
static uint64_t *prime_list = NULL;      /* list of small primes            */
static size_t prime_cnt = 0;             /* number of primes                 */
static size_t prime_cap = 0;             /* allocation capacity             */
static uint64_t candidate = 1;           /* next integer to test            */

/* Helper: enlarge cache array if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* out of memory */
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Helper: enlarge prime list array if needed */
static void ensure_prime_cap(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_arr) abort();               /* out of memory */
    prime_list = new_arr;
    prime_cap = new_cap;
}

/* Generate primes up to 'limit' inclusively, extending prime_list if necessary */
static void generate_primes_up_to(uint64_t limit)
{
    if (prime_cnt > 0 && prime_list[prime_cnt - 1] >= limit)
        return;                         /* already have enough */

    /* start sieve at 2 if list is empty */
    uint64_t start = 2;
    if (prime_cnt > 0) start = prime_list[prime_cnt - 1] + 1;

    /* Basic trial‑divide to generate new primes */
    for (uint64_t num = start; num <= limit; ++num) {
        bool is_prime = true;
        uint64_t r = (uint64_t)sqrt((double)num) + 1;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (num % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = num;
        }
    }
}

/* Compute σ(n) modulo 3 by factorisation, all arithmetic done mod 3 */
static int sigma_mod3(uint64_t n)
{
    if (n == 0) return 0;                /* not needed but safe */
    uint64_t temp = n;
    int result = 1;                      /* product of (1 + p + ... + p^e) mod 3 */

    generate_primes_up_to((uint64_t)sqrt((double)temp) + 1);

    for (size_t idx = 0; idx < prime_cnt && prime_list[idx] * prime_list[idx] <= temp; ++idx) {
        uint64_t p = prime_list[idx];
        if (temp % p != 0)
            continue;
        int e = 0;
        while (temp % p == 0) {
            ++e;
            temp /= p;
        }
        /* sum 1 + p + ... + p^e mod 3 */
        int sum_mod = 0;
        int term = 1 % 3;
        for (int i = 0; i <= e; ++i) {
            sum_mod = (sum_mod + term) % 3;
            term = (term * (p % 3)) % 3;
        }
        result = (result * sum_mod) % 3;
    }
    if (temp > 1) {                      /* remaining prime factor */
        int sum_mod = (1 + (int)(temp % 3)) % 3;
        result = (result * sum_mod) % 3;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, but guard */

    /* Grow cache until it contains at least n terms */
    while (cache_size < n) {
        /* apply filter condition */
        if (candidate % 3 != 0) {
            if (sigma_mod3(candidate) != 0) {
                ensure_cache_cap(cache_size + 1);
                seq_cache[cache_size++] = (seq_t)candidate;
                /* cache may become unsorted if we didn't care, but this preserves order */
            }
        }
        ++candidate;
    }
    return seq_cache[n - 1];
}
