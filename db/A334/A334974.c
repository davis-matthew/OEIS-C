/*
 * A334974  Infinitary admirable numbers: numbers k such that there is a
 *          proper infinitary divisor d of k such that isigma(k) - 2*d = 2*k,
 *          where isigma is the sum of infinitary divisors function (A049417).
 * https://oeis.org/A334974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334974 – infinitary admirable numbers.
 * a(n) is the nth integer k for which there exists a proper infinitary
 * divisor d of k satisfying: isigma(k) - 2*d = 2*k, where
 * isigma(k) is the sum of all infinitary divisors (A049417).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

/* Static cache of discovered sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms cached */
static size_t cache_cap = 0;   /* capacity of the cache */

/* Next candidate integer to test, starting with 2. */
static uint64_t next_candidate = 2;

/* Ensure cache has capacity for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Factorise n into primes and their exponents.
 * primes[] and exps[] must have room for all factors. */
static size_t factorise(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (t > 1) {
        primes[cnt] = t;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* Test whether k is an infinitary admirable number. */
static bool is_inf_admirable(uint64_t k)
{
    uint64_t primes[64];
    int      exps[64];
    size_t   pf_cnt = factorise(k, primes, exps);

    /* Compute isigma(k) = product over terms (1 + p^(2^j)) for each bit j set in exponent e. */
    seq_t isigma = (seq_t)1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];
        for (int j = 0; e >> j; ++j) {
            if (e & (1 << j)) {
                seq_t term = (seq_t)1 + pow_uint(p, 1ULL << j);
                isigma *= term;
            }
        }
    }

    /* Sum of proper infinitary divisors is isigma - k. */
    seq_t P = isigma - (seq_t)k;
    if (P <= (seq_t)k)
        return false;                     /* no proper divisor can satisfy the equation */

    seq_t diff = P - (seq_t)k;
    if ((diff % (seq_t)2) != (seq_t)0)
        return false;                     /* diff must be even */

    seq_t d = diff / (seq_t)2;
    if (d == (seq_t)0 || d >= (seq_t)k)
        return false;                     /* d must be proper */

    /* d must divide k. */
    if (d > (seq_t)UINT64_MAX)
        return false;
    uint64_t d64 = (uint64_t)d;
    if (k % d64 != 0)
        return false;

    /* d must be an infinitary divisor: its exponent bits are subset of those of k. */
    uint64_t dtmp = d64;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int exp_k = exps[i];
        int exp_d = 0;
        while (dtmp % p == 0) {
            dtmp /= p;
            ++exp_d;
        }
        if ((exp_d & ~exp_k) != 0)
            return false;                 /* a bit not present in k */
    }
    if (dtmp != 1)
        return false;                     /* contains a prime not dividing k */

    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 but never called */

    /* Fill cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_inf_admirable(next_candidate))
            grow_cache(cache_len + 1), cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
