/*
 * A340145  Dirichlet inverse of A247074(x) = phi(x)/(Product_{primes p
 *          dividing x} gcd(p-1, x-1)).
 * https://oeis.org/A340145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 116, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   factor_u64, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet inverse of A247074(n) = φ(n) / ∏_{p|n} gcd(p‑1, n‑1) */
/* The function a(n) satisfies ∑_{d|n} a(d) b(n/d) = δ_{n,1}  (Dirichlet inverse). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"

/* Compute b(n) = φ(n) / ∏_{p|n} gcd(p‑1, n‑1)   (n ≥ 1) */
static seq_t b_from_n(idx_t n)
{
    /* factor n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* φ(n) */
    seq_t phi = (seq_t)n;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        phi = phi / (seq_t)p * (seq_t)(p - 1);
    }

    /* denominator = ∏ gcd(p-1, n-1) */
    seq_t denom = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        seq_t g = gcd((seq_t)(p - 1), (seq_t)(n - 1));
        denom *= g;
    }

    return phi / denom;
}

/* ----- caching machinery ----- */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t cache_cap = 0;        /* number of allocated slots          */
static idx_t  computed_upto = 0;    /* largest n for which a(n) is known */

static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t old_cap = cache_cap;
    size_t new_cap  = ((size_t)n + 2) * 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    for (size_t i = old_cap; i < new_cap; ++i) {
        a_cache[i] = 0;
        b_cache[i] = 0;
    }
}

/* Dirichlet inverse of b: a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* undefined for n=0 */

    ensure_cache_size(n);

    /* base case n = 1 */
    if (computed_upto < 1) {
        a_cache[1] = 1;
        b_cache[1] = 1;
        computed_upto = 1;
    }

    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i == 1) continue;   /* already set */

        /* b(i) */
        b_cache[i] = b_from_n(i);

        /* Dirichlet inverse recurrence */
        seq_t sum = 0;
        size_t lim = (size_t)int_sqrt(i);
        for (size_t d = 1; d <= lim; ++d) {
            if (i % d != 0) continue;
            size_t e = (size_t)(i / d);

            if (d < (size_t)i)
                sum += a_cache[d] * b_cache[e];
            if (e < (size_t)i && e != d)
                sum += a_cache[e] * b_cache[d];
        }
        a_cache[i] = -sum;
    }

    computed_upto = n;
    return a_cache[n];
}
