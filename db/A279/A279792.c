/*
 * A279792  Number of Goldbach partitions (p,q) of 2n such that 0 < |p-q| < n.
 * https://oeis.org/A279792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered prime pairs (p,q) with p+q = 2n, 0 <
 *            |p‑q| < n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"   /* assumed to be available */

static idx_t      max_sieve_limit = 0;
static uint64_t  *prime_list       = NULL;
static size_t    prime_count       = 0;
static unsigned char *prime_bool   = NULL;
static size_t    prime_bool_size   = 0;

/* Helper to rebuild sieve up to at least limit */
static void rebuild_sieve(idx_t limit)
{
    /* free old structures */
    if (prime_list)   free(prime_list);
    if (prime_bool)   free(prime_bool);

    build_primes_upto(limit, &prime_list, &prime_count);

    prime_bool_size = (size_t)(limit + 1);
    prime_bool = (unsigned char*)calloc(prime_bool_size, sizeof(unsigned char));
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        prime_bool[(size_t)p] = 1;
    }

    max_sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t two_n = (idx_t)2 * n;
    if (two_n > max_sieve_limit) {
        rebuild_sieve(two_n);
    }

    seq_t count = 0;
    seq_t half  = (seq_t)(n / 2);

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p <= half || p >= n) continue;            /* ensure n/2 < p < n */
        seq_t q = (seq_t)two_n - p;                   /* q = 2n - p */
        if (prime_bool[(size_t)q]) ++count;           /* q is prime */
    }
    return count;
}
