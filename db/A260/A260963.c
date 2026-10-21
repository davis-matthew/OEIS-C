/*
 * A260963  Numbers k such that gcd(sigma(k), k*(k+1)/2 - sigma(k)) = 1, where
 *          sigma(k) is sum of positive divisors of k.
 * https://oeis.org/A260963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = kth integer k such that gcd(sigma(k), k*(k+1)/2 -
 *            sigma(k)) == 1
 * Range:     u8 n <= 8, u16 n <= 487, u32 n <= 8727, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sigma(k) is the sum of positive divisors of k. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static seq_t *seq_cache = NULL;   /* Array of found terms           */
static size_t seq_len   = 0;       /* Number of terms stored         */
static size_t seq_cap   = 0;       /* Allocated capacity             */
static idx_t  last_k    = 0;       /* Highest k processed so far     */

/* Compute sigma(k) using prime factorization.  The result fits in seq_t. */
static seq_t sigma_of_uint64(uint64_t n)
{
    if (n == 1)
        return 1;                 /* sigma(1) = 1 */
    /* temporary storage for factors */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 1;          /* sum of p^0 + p^1 + ... + p^e */
        seq_t pow  = 1;
        for (uint64_t j = 0; j < e; ++j) {
            pow  *= p;
            term += pow;
        }
        res *= term;
    }
    return res;
}

/* Ensure that seq_cache contains at least `needed` terms. */
static void ensure_cache(idx_t needed)
{
    if (seq_len >= needed)
        return;

    if (seq_cap == 0) {
        seq_cap = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else {
        while (seq_cap < needed)
            seq_cap <<= 1;               /* double capacity */
        seq_cache = (seq_t *)realloc(seq_cache,
                                     seq_cap * sizeof(seq_t));
    }

    for (idx_t k = last_k + 1; ; ++k) {
        seq_t kk    = (seq_t)k;
        seq_t sigma = sigma_of_uint64((uint64_t)k);
        seq_t tri   = kk * (kk + 1) / 2;
        seq_t diff  = tri - sigma;
        seq_t g     = gcd(sigma, diff);
        if (g == 1) {
            seq_cache[seq_len++] = kk;
            if (seq_len >= needed) {
                last_k = k;
                return;
            }
        }
        if (k == UINT64_MAX)       /* safety, should never happen */
            return;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based (offset 1). Return 0 for n==0 to avoid UB. */
    if (n == 0)
        return 0;
    ensure_cache(n);
    return seq_cache[n - 1];
}
