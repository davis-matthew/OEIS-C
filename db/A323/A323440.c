/*
 * A323440  Numbers divisible by exactly one of their distinct prime indices.
 * https://oeis.org/A323440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   realloc_cast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323440: numbers divisible by exactly one of their distinct prime indices
   The function generates the terms incrementally, caching them.
   A number m satisfies the condition iff, for every distinct prime factor p of m,
   let i(p) be the index of p in the list of primes (2 ->1, 3 ->2, 5->3, ...).
   Then exactly one of these indices i(p) divides m. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/realloc_cast.h"

static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t newcap = (*cap) ? (*cap) << 1 : 16;
        while (newcap < min_cap) newcap <<= 1;
        *arr = (uint64_t *)realloc_cast(*arr, newcap * sizeof(**arr), "uint64_t");
        *cap = newcap;
    }
}

static void grow_seq_array(seq_t **arr, size_t *cap, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t newcap = (*cap) ? (*cap) << 1 : 16;
        while (newcap < min_cap) newcap <<= 1;
        *arr = (seq_t *)realloc_cast(*arr, newcap * sizeof(**arr), "seq_t");
        *cap = newcap;
    }
}

/* ---------- integer square root ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        if (mid > n / mid) { /* mid*mid > n */
            high = mid - 1;
        } else {
            uint64_t sq = mid * mid;
            if (sq == n) return mid;
            if (sq < n) low = mid + 1;
            else high = mid - 1;
        }
    }
    return high;
}

/* ---------- prime list ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0, prime_cnt = 0;

/* add one prime to the list */
static void add_prime(uint64_t p)
{
    grow_uint64_array(&prime_list, &prime_cap, prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* generate primes up to at least 'limit' (inclusive) */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt == 0) {
        add_prime(2);
    }
    uint64_t last = prime_list[prime_cnt - 1];
    if (last >= limit) return;

    uint64_t candidate = last + 1;
    if (candidate == 2) candidate = 2;
    else if (candidate % 2 == 0) candidate++;

    while (last < limit) {
        bool isPrime = true;
        uint64_t sqrtC = isqrt_u64(candidate);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrtC; ++i) {
            if (candidate % prime_list[i] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            add_prime(candidate);
            last = candidate;
        }
        candidate += 2;
    }
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0, seq_len = 0;
static uint64_t last_checked = 1;

/* helper to get prime index (1‑based) for a known prime */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)(lo + 1);
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety */

    /* extend cache if needed */
    if (seq_len < (size_t)n) {
        grow_seq_array(&seq_cache, &seq_cap, (size_t)n);

        uint64_t m = last_checked + 1;
        while (seq_len < (size_t)n) {
            /* ensure primes for factoring */
            uint64_t limit = isqrt_u64(m);
            ensure_primes_upto(limit);

            uint64_t temp = m;
            size_t div_cnt = 0;

            /* factor using primes up to sqrt(temp) */
            for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
                uint64_t p = prime_list[i];
                if (temp % p == 0) {
                    idx_t idx = (idx_t)(i + 1);
                    if (m % idx == 0) ++div_cnt;
                    while (temp % p == 0) temp /= p;
                }
            }

            /* remaining prime factor if any */
            if (temp > 1) {
                /* ensure that prime list contains temp (it might be > last prime) */
                ensure_primes_upto(temp);
                idx_t idx = prime_index(temp);
                if (m % idx == 0) ++div_cnt;
            }

            if (div_cnt == 1) {
                seq_cache[seq_len++] = (seq_t)m;
            }

            ++m;
        }
        last_checked = m - 1;
    }

    return seq_cache[(size_t)n - 1];
}
