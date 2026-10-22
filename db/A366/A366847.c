/*
 * A366847  Numbers whose halved even prime indices are nonempty and
 *          relatively prime.
 * https://oeis.org/A366847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer whose halved even prime indices have
 *            greatest common divisor equal to 1 (and the set is nonempty)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/*---------------------------------------------------------------
  Prime list and utilities
  ---------------------------------------------------------------*/
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

static void extend_primes_to(uint64_t limit)
{
    if (prime_cnt > 0 && prime_list[prime_cnt-1] >= limit)
        return;

    if (prime_cnt == 0) {
        prime_cap = 256;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    uint64_t cand = prime_list[prime_cnt-1] + 1;
    for ( ; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt &&
                         prime_list[i] <= cand / prime_list[i]; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* Binary search: index (0‑based) of a known prime in prime_list.  */
static size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* prime_list[lo] == p  */
}

/* Integer square root (floor) for uint64_t.  */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Test whether a given number satisfies the sequence condition.  */
static bool is_valid(uint64_t n)
{
    bool has_even = false;
    seq_t gcd_all = 0;               /* 0 indicates not yet set */
    uint64_t temp = n;

    /* Ensure prime table up to sqrt(n).  */
    extend_primes_to(isqrt_u64(n));

    /* Factor using pre‑computed primes.  */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= temp / prime_list[i];
         ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            /* Consume the prime factor (exponent irrelevant). */
            do { temp /= p; } while (temp % p == 0);

            size_t idx = i + 1; /* 1‑based index of prime p */
            if ((idx & 1) == 0) { /* even index */
                uint64_t k = idx >> 1;   /* halved index */

                if (gcd_all == 0)
                    gcd_all = (seq_t)k;
                else
                    gcd_all = gcd(gcd_all, (seq_t)k);

                has_even = true;
            }
        }
    }

    /* Remaining prime factor (if any).  */
    if (temp > 1) {
        extend_primes_to(temp);
        size_t idx = prime_index_of(temp) + 1;   /* 1‑based */
        if ((idx & 1) == 0) {
            uint64_t k = idx >> 1;
            if (gcd_all == 0)
                gcd_all = (seq_t)k;
            else
                gcd_all = gcd(gcd_all, (seq_t)k);

            has_even = true;
        }
    }

    return has_even && gcd_all == 1;
}

/*---------------------------------------------------------------
  Sequence cache and main routine
  ---------------------------------------------------------------*/
static seq_t   *seq_cache   = NULL;
static size_t   seq_cnt     = 0;
static size_t   seq_cap     = 0;
static uint64_t next_candidate = 2;   /* will be incremented to 3 first */
static bool     initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        extend_primes_to(2);
        seq_cap = 32;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cnt = 0;
        next_candidate = 2;
    }

    while (seq_cnt < (size_t)n) {
        /* Find next valid integer. */
        do {
            ++next_candidate;
        } while (!is_valid(next_candidate));

        /* Store the found term. */
        if (seq_cnt == seq_cap) {
            seq_cap <<= 1;
            seq_cache = (seq_t *)realloc(seq_cache,
                                         seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_cnt++] = (seq_t)next_candidate;
    }

    return seq_cache[n - 1];
}
