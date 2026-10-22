/*
 * A359823  Dirichlet inverse of A359820, where A359820 is the characteristic
 *          function of numbers whose parity differs from the parity of their
 *          arithmetic derivative (A003415).
 * https://oeis.org/A359823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, and for n > 1, a(n) = -∑_{d|n, d < n} A359820(n/d) ·
 *            a(d), where A359820(m) = 1 if parity(m) differs from
 *            parity(D(m)), D(m) is the arithmetic derivative
 * Range:     i8 n <= 9379, i16 n <= 9379, i32 n <= 9379, i64 n <= 9379,
 *            i128 n <= 9379, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* caches for a(n) (1-indexed) and f(n)=A359820(n) */
static seq_t   *a_cache = NULL;
static int     *f_cache = NULL;
static idx_t    cache_size = 0;

/* Parity of arithmetic derivative of n (stays in 64‑bit for n <= 1 000 000) */
static int compute_f(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute D(n) using the product rule */
    unsigned long long D = 0;          /* derivative so far   */
    unsigned long long prod = 1;       /* product of factors so far */

    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p   = primes[i];
        unsigned long long e   = exps[i];

        /* D(p^e) = e * p^{e-1} */
        unsigned long long pow = 1;
        for (unsigned long long j = 1; j < e; ++j) pow *= p;   /* p^{e-1} */
        unsigned long long D_pe = e * pow;                     /* integer */

        /* new product: prod * p^e */
        unsigned long long new_prod = prod * pow * p;          /* = prod * p^e */

        /* update derivative: D = D* p^e + prod * D(p^e) */
        unsigned long long new_D = D * pow * p + prod * D_pe;

        prod = new_prod;
        D    = new_D;
    }

    /* parity test */
    int parity_n = n & 1;
    int parity_D = D & 1;
    return (parity_n != parity_D) ? 1 : 0;
}

/* expand caches up to and including n, computing values as needed */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n) return;

    idx_t new_size = n;
    a_cache = (seq_t *)realloc(a_cache, (new_size + 1) * sizeof(seq_t));
    f_cache = (int *)realloc(f_cache, (new_size + 1) * sizeof(int));

    /* compute f for new indices */
    for (idx_t i = cache_size + 1; i <= new_size; ++i) {
        f_cache[i] = compute_f(i);
    }

    /* compute a for new indices */
    for (idx_t i = cache_size + 1; i <= new_size; ++i) {
        if (i == 1) {
            a_cache[1] = 1;                 /* base case */
            continue;
        }
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;

                if (d1 < i) {
                    sum -= (seq_t)f_cache[i / d1] * a_cache[d1];
                }
                if (d2 != d1 && d2 < i) {
                    sum -= (seq_t)f_cache[i / d2] * a_cache[d2];
                }
            }
        }
        a_cache[i] = sum;
    }
    cache_size = new_size;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */
    ensure_cache(n);
    return a_cache[n];
}
