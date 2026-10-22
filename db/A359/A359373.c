/*
 * A359373  Nonmultiples of 4 that have an odd number of prime factors (with
 *          multiplicity).
 * https://oeis.org/A359373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359373: nonmultiples of 4 with an odd number of prime factors (with multiplicity)
   Computed by scanning successive integers; no closed formula. */

/* Required headers */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t n) */

static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Append a prime to prime_list, expanding the array if necessary. */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t newcap = prime_cap ? prime_cap * 2 : 4;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);          /* memory exhausted */
        prime_list = tmp;
        prime_cap  = newcap;
    }
    prime_list[prime_cnt++] = p;
}

/* Generate all primes ≤ limit, filling prime_list. */
static void ensure_primes_leq(uint64_t limit)
{
    if (prime_cnt == 0) add_prime(2);          /* start with 2 */

    uint64_t candidate = prime_list[prime_cnt - 1] + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate % 2 == 0) candidate++;        /* skip evens */

    while (candidate <= limit) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) { isprime = false; break; }
        }
        if (isprime) add_prime(candidate);
        candidate += 2;
    }
}

/* Count the parity (odd/even) of the number of prime factors of n (with multiplicity).
   Returns 1 if odd, 0 if even. */
static int odd_prime_factor_parity(uint64_t n)
{
    uint64_t temp = n;
    unsigned parity = 0;    /* 0 = even, 1 = odd */

    /* Ensure we have primes up to sqrt(temp) for factoring */
    uint64_t sqrt_n = _floor_isqrt_u64(temp);
    if (prime_cnt == 0 || prime_list[prime_cnt - 1] < sqrt_n)
        ensure_primes_leq(sqrt_n);

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        while (temp % p == 0) {
            parity ^= 1;                     /* toggle parity per factor */
            temp /= p;
        }
    }
    if (temp > 1) parity ^= 1;                 /* remaining prime factor */

    return (int)parity;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 -- value for n=0 is unused */

    uint64_t found = 0;
    uint64_t cand  = 2;

    while (found < n) {
        if ((cand & 3) != 0) {                     /* skip multiples of 4 */
            if (odd_prime_factor_parity(cand)) {
                ++found;
                if (found == n) return (seq_t)cand;
            }
        }
        ++cand;
    }
    return (seq_t)0;   /* unreachable, keeps compiler happy */
}
