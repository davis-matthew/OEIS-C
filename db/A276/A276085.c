/*
 * A276085  Primorial base log-function: fully additive with a(p) = p#/p,
 *          where p# = A034386(p).
 * https://oeis.org/A276085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e ∥ n} e * (product of all primes < p). For n = 1,
 *            a(1) = 0
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes_upto, factor_u64, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static seq_t *prev_primorial = NULL;

static void ensure_primes_upto(idx_t limit) {
    if (prime_count > 0 && limit <= prime_list[prime_count - 1])
        return;
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(prime_list);
    if (prev_primorial) {
        free_seq_array(prev_primorial);
    }
    prime_list = new_list;
    prime_count = new_cnt;
    prev_primorial = alloc_seq_array(prime_count);
    seq_t prod = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        prev_primorial[i] = prod;          /* product of primes < prime_list[i]  */
        prod = prod * ((seq_t)prime_list[i]); /* multiply by current prime */
    }
}

static int index_of_prime(uint64_t prime) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == prime)
            return (int)mid;
        else if (prime_list[mid] < prime)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1; /* should not happen for prime factors of n */
}

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    uint64_t u = (uint64_t)n;
    uint64_t facs[64];
    uint64_t exps[64];
    size_t fcnt = 0;

    factor_u64(u, facs, exps, &fcnt);

    uint64_t max_p = 0;
    for (size_t i = 0; i < fcnt; ++i)
        if (facs[i] > max_p)
            max_p = facs[i];
    if (max_p > 0)
        ensure_primes_upto(max_p);

    seq_t result = 0;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = facs[i];
        size_t e = (size_t)exps[i];
        int idx = index_of_prime(p);
        if (idx < 0)
            continue;
        seq_t val = prev_primorial[idx];
        result += ((seq_t)e) * val;
    }

    return result;
}
