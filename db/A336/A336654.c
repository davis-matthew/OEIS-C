/*
 * A336654  Numbers k such that lambda(k) is squarefree, where lambda is the
 *          Carmichael lambda function (A002322).
 * https://oeis.org/A336654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, is_squarefree, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336654: Numbers k such that Carmichael λ(k) is squarefree.
   The function searches sequentially for integers whose λ(k) is
   square‑free and memoises the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/is_squarefree.h"/* bool is_squarefree(seq_t x);   */
#include "../utils/seq_pow.h"      /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* --------------------  static cache data  -------------------------- */
static uint64_t          next_candidate = 1;   /* next integer to test          */
static seq_t            *term_cache     = NULL;  /* stored sequence values        */
static size_t            cache_len      = 0;     /* number of values stored       */
static size_t            cache_cap      = 0;     /* allocated capacity            */

/* ------------------------------------------------------------------ */
/* Compute Carmichael λ for a 64‑bit integer.                          */
/* Uses factor_u64 and the known formula for λ(p^e).                   */
/* ------------------------------------------------------------------ */
static seq_t lambda_from_uint64(uint64_t n)
{
    if (n == 1) return (seq_t)1;                 /* λ(1)=1 */

    /* factor_u64 returns primes, exponents and count. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2ULL) {
            if (e == 1ULL)
                part = (seq_t)1;
            else if (e == 2ULL)
                part = (seq_t)2;
            else
                part = seq_pow((seq_t)2, (idx_t)(e - 2));  /* 2^(e-2) */
        } else {
            part = seq_pow((seq_t)p, (idx_t)(e - 1));
            part = part * (seq_t)(p - 1);                 /* p^(e-1)*(p-1) */
        }

        seq_t g = gcd(result, part);
        result = (result / g) * part;
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* Ensure the cache contains at least n terms.                         */
/* Expands progressively as required.                                 */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        seq_t lam = lambda_from_uint64(next_candidate);

        if (is_squarefree(lam)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);                      /* allocation error   */
                term_cache = tmp;
                cache_cap   = new_cap;
            }
            term_cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Public API: return the n‑th term (1‑based indexing).                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined case          */
    ensure_terms(n);                /* make cache long enough  */
    return term_cache[n - 1];       /* zero‑based internal     */
}
