/*
 * A348976  Möbius transform of A129283, which is sum of n and its arithmetic
 *          derivative.
 * https://oeis.org/A348976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mobius(n/d) * A129283(d) (OEIS A348976)
 * Range:     u8 n <= 127, u16 n <= 16383, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Recursive computation of the Möbius transform.
 *
 * Arguments:
 *   idx      : current prime index being processed
 *   cnt      : total number of distinct primes in n
 *   pr       : array of prime bases
 *   exp_n    : array of exponents in n
 *   prod     : product of the chosen divisor d so far
 *   sign     : current Möbius sign (+1 or -1)
 *   sum      : accumulated Möbius weighted sum
 *   e_d      : array holding the exponents of d for every prime
 */
static void rec_mu(size_t idx, size_t cnt,
                   const uint64_t *pr, const uint64_t *exp_n,
                   seq_t prod, int sign,
                   seq_t *sum, uint64_t *e_d)
{
    if (idx == cnt) {
        /* compute A129283(d) = d + sum_{i} e_i * d / p_i */
        seq_t derivative = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (e_d[i] > 0) {
                seq_t term = (seq_t)e_d[i] * prod / (seq_t)pr[i];
                derivative += term;
            }
        }
        seq_t a129283 = prod + derivative;
        if (sign == 1) {
            *sum += a129283;
        } else {
            *sum -= a129283;   /* subtract when Möbius is -1 */
        }
        return;
    }

    /* Option 1: take full exponent of this prime (quotient exponent = 0) */
    seq_t prod_full = prod;
    for (uint64_t e = 0; e < exp_n[idx]; ++e) {
        prod_full *= (seq_t)pr[idx];
    }
    e_d[idx] = exp_n[idx];
    rec_mu(idx + 1, cnt, pr, exp_n, prod_full, sign, sum, e_d);

    /* Option 2: take one less exponent (quotient exponent = 1), flips sign */
    if (exp_n[idx] >= 1) {
        seq_t prod_partial = prod;
        for (uint64_t e = 0; e < exp_n[idx] - 1; ++e) {
            prod_partial *= (seq_t)pr[idx];
        }
        e_d[idx] = exp_n[idx] - 1;
        rec_mu(idx + 1, cnt, pr, exp_n, prod_partial, -sign, sum, e_d);
    }
    e_d[idx] = 0;   /* restore for caller */
}

/* Small factorisation for uint64_t numbers.  The array sizes are chosen
 * to handle all numbers that may appear in a typical test case (≤ 2^64). */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c]   = p;
            exp[c++] = e;
        }
    }
    if (n > 1) {               /* remaining prime factor */
        pr[c]   = n;
        exp[c++] = 1;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Cache used to avoid recomputation for repeated calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure the cache array can hold the requested index */
    ensure_cache(n, &cache, &cap);

    if (cache[n] != 0)            /* cached value is non‑zero (sequence > 0) */
        return cache[n];

    /* Factor n once – needed for the divisor enumeration */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Temporary storage for the exponents of the current divisor */
    uint64_t e_d[16] = {0};

    seq_t sum = 0;
    rec_mu(0, cnt, primes, exps, (seq_t)1, 1, &sum, e_d);

    cache[n] = sum;
    return sum;
}
