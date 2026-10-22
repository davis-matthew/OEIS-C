/*
 * A387992  E.g.f. A(x) satisfies A(x) = exp( x^2*A(x)^2 * (1+x*A(x))^2 ).
 * https://oeis.org/A387992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n+1)^(k-1) *
 *            binomial(2k,n-2k)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* ---- factorial cache ---- */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;
    static idx_t fact_len = 0;   /* number of factorials stored (0 .. fact_len-1) */

    if (fact_len == 0) {
        fact_cap = 16;
        fact = (seq_t*)malloc(fact_cap * sizeof(seq_t));
        fact[0] = 1u;            /* 0! = 1 */
        fact_len = 1;
    }
    if (n >= fact_len) {
        size_t need = (size_t)(n + 1);
        if (need > fact_cap) {
            size_t new_cap = fact_cap * 2;
            while (new_cap < need) new_cap *= 2;
            fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
            fact_cap = new_cap;
        }
        for (idx_t i = fact_len; i <= n; ++i) {
            fact[i] = seq_mul(fact[i - 1], (seq_t)i);
        }
        fact_len = n + 1;
    }

    /* ---- term cache ---- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* next index to compute */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if ((size_t)n >= cache_cap) {
        size_t need = (size_t)(n + 1);
        size_t new_cap = cache_cap * 2;
        while (new_cap < need) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                     /* a(0) = 1 */
            cache[0] = 1u;
            continue;
        }
        seq_t sum = 0;
        idx_t maxk = i / 2;
        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t pow_term  = seq_pow((seq_t)(i + 1), (idx_t)(k - 1));          /* (n+1)^(k-1) */
            seq_t binom     = binomial(2 * k, i - 2 * k);                       /* C(2k, n-2k) */
            seq_t temp      = fact[i] / fact[k];                                /* n! / k! */

            seq_t term = seq_mul(pow_term, binom);   /* (n+1)^(k-1) * binom */
            term = seq_mul(term, temp);              /* * (n! / k!) */

            sum += term;
        }
        cache[i] = sum;                      /* already includes n! */
    }
    cache_len = n + 1;
    return cache[n];
}
