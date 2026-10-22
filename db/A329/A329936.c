/*
 * A329936  Binary hoax numbers: composite numbers k such that sum of bits of
 *          k equals the sum of bits of the distinct prime divisors of k.
 * https://oeis.org/A329936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329936: binary hoax numbers
 *  a(n) is the n‑th composite integer k such that
 *      popcount(k) = Σ popcount(p)
 *  where the sum is over the distinct prime divisors p of k.
 *  The sequence starts at 4 (offset 1).  It is computed by
 *  searching successive integers and testing the popcount condition. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* ------------------------------------------------------------------
 * Generic popcount for any seq_t that supports bitwise &, >>
 * ------------------------------------------------------------------ */
static inline int popcount_seq_t(seq_t x)
{
    int cnt = 0;
    while (x) {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* ------------------------------------------------------------------
 * Prime storage and generation (uint64_t primes)
 * ------------------------------------------------------------------ */
static uint64_t *prime_arr = NULL;
static size_t     prime_len = 0, prime_cap = 0;

static void prime_grow(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 4;
    uint64_t *p = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!p) exit(1);
    prime_arr = p;
    prime_cap = new_cap;
}

static void generate_next_prime(void)
{
    if (prime_len == 0) {
        prime_grow();
        prime_arr[0] = 2;
        prime_len = 1;
        return;
    }
    uint64_t cand = prime_arr[prime_len - 1] + 1;
    for (;;) {
        bool is_p = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_arr[i];
            if (p > cand / p) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_len == prime_cap) prime_grow();
            prime_arr[prime_len++] = cand;
            break;
        }
        ++cand;
    }
}

static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_len == 0) generate_next_prime();
    while (prime_arr[prime_len - 1] <= limit) generate_next_prime();
}

/* ------------------------------------------------------------------
 * Sequence cache
 * ------------------------------------------------------------------ */
static seq_t *cache_arr = NULL;
static idx_t  cache_len = 0, cache_cap = 0;

static void cache_grow(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    cache_arr = p;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------
 * Main function requested by the specification
 * ------------------------------------------------------------------ */
static uint64_t next_candidate = 4;         /* start checking from 4 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset */

    while (cache_len < n) {
        uint64_t k = next_candidate++;
        /* fast composite test: if no factor found below sqrt(k), skip */
        uint64_t sqrtk = (uint64_t)floor_sqrt((seq_t)k);
        ensure_primes_up_to(sqrtk);

        uint64_t tmp = k;
        int pop_sum = 0;
        bool composite = false;

        for (size_t i = 0; i < prime_len && prime_arr[i] * prime_arr[i] <= tmp; ++i) {
            uint64_t p = prime_arr[i];
            if (tmp % p == 0) {
                composite = true;
                pop_sum += popcount_seq_t((seq_t)p);
                while (tmp % p == 0) tmp /= p;
            }
        }
        if (tmp > 1) {
            if (tmp != k) composite = true;      /* factor > sqrt(k) */
            pop_sum += popcount_seq_t((seq_t)tmp);
        }

        if (composite && popcount_seq_t((seq_t)k) == pop_sum) {
            cache_grow(cache_len + 1);
            cache_arr[cache_len++] = (seq_t)k;
        }
    }
    return cache_arr[n - 1];
}
