/*
 * A347157  Sum of cubes of distinct prime divisors of n that are < sqrt(n).
 * https://oeis.org/A347157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A347157 – Sum of cubes of distinct prime divisors of n that are < sqrt(n)
 * Formula: a(n) = Σ_{p|n, p<√n} p³
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Integer square root : floor(√n) */
static idx_t floor_sqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq == (uint64_t)n)
            return mid;
        if (sq < (uint64_t)n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Cache of primes up to the largest limit requested */
static uint64_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static idx_t       prime_limit = 0;

/* Extend the cached prime list to at least `limit` */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* For n < 4, sqrt(n) < 2 so no prime < sqrt(n) exists */
    if (n < 4)
        return 0;

    idx_t limit = floor_sqrt_idx(n);
    ensure_primes(limit);

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if ((n % p) == 0) {
            /* Include only if p < sqrt(n)  <=>  p*p < n */
            if ((uint64_t)p * p < n) {
                seq_t cube = (seq_t)p * p * p;
                sum += cube;
            }
        }
    }
    return sum;
}
