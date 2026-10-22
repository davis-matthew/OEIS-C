/*
 * A363261  The partial sums of the prime indices of n include half the sum of
 *          all prime indices of n.
 * https://oeis.org/A363261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A363261:
   a(n) = the n‑th positive integer N such that the sorted multiset of
   prime indices of N contains a prefix sum equal to half of the total
   sum of its prime indices. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PRIME_LIMIT 500000u

static uint32_t primes[MAX_PRIME_LIMIT/2 + 1];
static uint32_t prime_index[MAX_PRIME_LIMIT + 1];
static bool is_composite[MAX_PRIME_LIMIT + 1];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    memset(is_composite, 0, sizeof(is_composite));
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            prime_index[i] = (uint32_t)prime_cnt;
            if ((uint64_t)i * i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    done = true;
}

static bool satisfies(idx_t N)
{
    if (N < 2) return false;
    idx_t tmp = N;
    seq_t total = 0;
    /* maximum multiplicities < 64 for numbers < MAX_PRIME_LIMIT */
    uint32_t idx_list[64];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            uint32_t idx = prime_index[p];
            while (tmp % p == 0) {
                idx_list[idx_cnt++] = idx;
                total += idx;
                tmp /= p;
            }
        }
    }
    if (tmp > 1) {
        /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        if (p > MAX_PRIME_LIMIT) return false; /* safety */
        uint32_t idx = prime_index[p];
        idx_list[idx_cnt++] = idx;
        total += idx;
    }
    if ((total & 1) != 0) return false;
    seq_t half = total >> 1;
    seq_t sum = 0;
    for (size_t i = 0; i < idx_cnt; ++i) {
        sum += idx_list[i];
        if (sum == half) return true;
        if (sum > half) break;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t next_num = 2;
    init_primes();
    while (cache_size < n) {
        while (!satisfies(next_num)) ++next_num;
        cache = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        cache[cache_size++] = (seq_t)next_num;
        ++next_num;
    }
    return cache[n - 1];
}
