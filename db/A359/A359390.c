/*
 * A359390  Sequence lists the numbers k such that bottom entry is an integer
 *          in the ratio d(i+1)/d(i) triangle of the elements in the divisors
 *          of n, where d(1) < d(2) < ... < d(q) denote the divisors of k.
 * https://oeis.org/A359390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   cmp_uint64_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers k for which the bottom entry
   of the ratio triangle built from the divisors d(i+1)/d(i) is an
   integer.  Equivalently, for each prime p|k the weighted sum
   Σ_i C(q-1,i-1)·(-1)^(q-i)·v_p(d_i) must be non‑negative, where
   d_i are the sorted divisors of k and q=τ(k).  The algorithm
   generates the terms by a simple test of each candidate number,
   recording them in a cache so that calls for arbitrary n are
   O(1) after the terms are generated. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_uint64_t.h"

/* recursive generation of all divisors from the prime factorisation */
static void gen_divs(int pos, uint64_t cur,
                     const int *primes, const int *exps,
                     int pcnt, uint64_t *divs, size_t *idx)
{
    if (pos == pcnt) {
        divs[(*idx)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exps[pos]; ++e) {
        gen_divs(pos + 1, val, primes, exps, pcnt, divs, idx);
        val *= (uint64_t)primes[pos];
    }
}

/* test whether the bottom entry of the ratio triangle of n is an integer */
static bool bottom_integer(uint64_t n)
{
    if (n <= 1)               /* 1 has divisors [1] and product=1 */
        return true;

    /* 1. factorise n */
    int primes[64];
    int exps[64];
    int pcnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = (int)p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = (int)tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* 2. enumerate all divisors */
    size_t q = 1;
    for (int i = 0; i < pcnt; ++i)
        q *= (size_t)(exps[i] + 1);
    uint64_t *divs = (uint64_t *)malloc(q * sizeof(uint64_t));
    if (!divs)
        return false;
    size_t pos = 0;
    gen_divs(0, 1, primes, exps, pcnt, divs, &pos);

    /* 3. sort the divisors */
    qsort(divs, q, sizeof(uint64_t), cmp_uint64_t);

    /* 4. pre‑compute weights e[i] = C(q-1,i-1)·(-1)^(q-i) */
    __int128 *e = (__int128 *)malloc(q * sizeof(__int128));
    if (!e) {
        free(divs);
        return false;
    }
    for (size_t idx = 0; idx < q; ++idx) {
        /* idx is 0‑based for i = idx+1 */
        int i = (int)idx;
        int r = i;
        int nq = (int)(q - 1);
        if (r > nq - r)
            r = nq - r;
        __int128 comb = 1;
        for (int t = 1; t <= r; ++t)
            comb = comb * (__int128)(nq - r + t) / t;
        int sign = ((q - (i + 1)) % 2 == 0) ? 1 : -1;
        e[idx] = comb * sign;
    }

    /* 5. weighted sums for each prime factor */
    for (int pj = 0; pj < pcnt; ++pj) {
        int p = primes[pj];
        __int128 sum = 0;
        for (size_t idx = 0; idx < q; ++idx) {
            uint64_t d = divs[idx];
            int exp = 0;
            while (d % p == 0) {
                d /= p;
                ++exp;
            }
            sum += e[idx] * exp;
        }
        if (sum < 0) {
            free(e);
            free(divs);
            return false;
        }
    }

    free(e);
    free(divs);
    return true;
}

/* cache the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;   /* start from 0 so that 1 is first tested */

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (bottom_integer(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0; /* memory failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
