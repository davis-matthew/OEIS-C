/*
 * A369329  Numbers whose neighbors have a prime number as their greatest odd
 *          divisor.
 * https://oeis.org/A369329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A369329: a(n) is the nth positive integer m≥4 
   whose neighbors m−1 and m+1 have a prime as their greatest odd divisor. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool isPrimeULL(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i <= x / i; i += 2)
        if (x % i == 0) return false;
    return true;
}

static uint64_t odd_part(uint64_t n)
{
    while ((n & 1) == 0) n >>= 1;
    return n;
}

static bool odd_part_is_prime(uint64_t n)
{
    uint64_t o = odd_part(n);
    return o > 1 && isPrimeULL(o);
}

static bool qualifies(uint64_t m)
{
    return odd_part_is_prime(m - 1) && odd_part_is_prime(m + 1);
}

/* dynamic cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3; /* next m to test (will be incremented to 4) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety; sequence is 1-indexed */
    while (cache_len < n) {
        uint64_t m = ++next_candidate;
        if (qualifies(m)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
