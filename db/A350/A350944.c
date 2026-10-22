/*
 * A350944  Heinz numbers of integer partitions of which the number of odd
 *          parts is equal to the number of odd conjugate parts.
 * https://oeis.org/A350944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz numbers of integer partitions with equal number of
 *            odd parts and odd conjugate parts. The function enumerates
 *            integers, tests the partition property, and memoises found
 *            values
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   _floor_isqrt_u64, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"
#include "../utils/_floor_isqrt_u64.h"

/* Prime list management -------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* 1‑based: prime_list[0] = 2 */
static size_t prime_count = 0;
static uint64_t prime_max = 0;         /* largest prime stored */

/* Ensure primes up to at least n are in prime_list */
static void ensure_primes_up_to(uint64_t n)
{
    if (n <= prime_max) return;
    size_t cnt = 0;
    idx_t *tmp = NULL;
    sieve_primes(n, &tmp, &cnt);                /* creates list 1‑based */
    free(prime_list);                           /* discard old list */
    prime_list = (uint64_t *)tmp;
    prime_count = cnt;
    prime_max   = (cnt > 0) ? prime_list[cnt - 1] : 0;
}

/* Return 1‑based index of a prime number; prime_list must contain it. */
static uint64_t prime_index(uint64_t p)
{
    if (p > prime_max) ensure_primes_up_to(p);
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else                    hi = mid;
    }
    /* prime_list[lo] == p */
    return lo + 1;
}

/* Factor a 64‑bit integer into prime factors and exponents.
   The arrays part[64], exp[64] are filled, and *cnt is set. */
static void factor_uint64(uint64_t n, uint64_t part[64], uint32_t exp[64], size_t *cnt)
{
    size_t c = 0;
    uint64_t rem = n;
    if (prime_count == 0) ensure_primes_up_to(2);  /* initialise primes */

    uint64_t lim = _floor_isqrt_u64(rem);
    ensure_primes_up_to(lim);

    for (size_t i = 0; i < prime_count && prime_list[i] <= lim; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            part[c] = p;
            exp[c]  = e;
            ++c;
            lim = _floor_isqrt_u64(rem);
        }
    }
    if (rem > 1) {                    /* remaining prime factor */
        if (rem > prime_max) ensure_primes_up_to(rem);
        part[c] = rem;
        exp[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* Test whether an integer satisfies the partition property. */
static bool satisfies_property(uint64_t m)
{
    if (m == 1) return true;

    uint64_t part[64];
    uint32_t exp[64];
    size_t cnt = 0;
    factor_uint64(m, part, exp, &cnt);

    /* Transform prime values to part indices (prime(y_i)). */
    uint64_t part_idx[64];
    for (size_t i = 0; i < cnt; ++i)
        part_idx[i] = prime_index(part[i]);

    /* Compute r_odd: total number of odd parts in the partition. */
    uint64_t r_odd = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (part_idx[i] & 1ULL) r_odd += exp[i];

    /* Compute c_odd: number of odd parts in the conjugate partition. */
    uint64_t sum_high = 0;     /* sum of exponents of parts greater than current */
    uint64_t c_odd    = 0;
    uint64_t next = part_idx[cnt - 1] + 1;   /* value just above max part */

    for (long long ii = (long long)cnt - 1; ii >= 0; --ii) {
        uint64_t p = part_idx[ii];
        uint64_t block_len = (next > p + 1) ? (next - p - 1) : 0ULL;
        if (block_len && (sum_high & 1))
            c_odd += block_len;

        /* j = p */
        if (((sum_high + exp[ii]) & 1))
            c_odd += 1;

        sum_high += exp[ii];
        next = p;
    }
    /* Remaining j from 1 to next-1 */
    if (sum_high & 1)
        c_odd += next - 1ULL;

    return r_odd == c_odd;
}

/* Memoised sequence storage ---------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t current = 1;  /* next integer to test */

/* Exported sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined as 0 for safety */

    /* Initialise prime list if not yet done */
    if (prime_count == 0) ensure_primes_up_to(2);

    while (cache_len < (size_t)n) {
        if (satisfies_property(current)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 8;
                cache    = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)current;
        }
        ++current;
    }
    return cache[(size_t)n - 1];
}
