/*
 * A258456  Product of divisors of n is not a square.
 * https://oeis.org/A258456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(k) is the k‑th natural number n for which the product of all
 *            divisors of n is not a perfect square. The sequence is generated
 *            by an incremental sieve testing each candidate integer
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/*  Determine whether the product of divisors of n is NOT a square.  */
static bool
is_non_square_product(uint64_t n)
{
    /*  Factor n.  We use 64‑bit arrays; |n| <= 2^64‑1 in all tests.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    bool is_square = true;       /* all exponents even?   */
    bool has_exp2mod4 = false;   /* at least one exp ≡ 2 (mod 4)  */
    int tau_mod4 = 1;            /* τ(n) modulo 4          */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if ((e & 1ULL) != 0ULL)        /* odd exponent → not square */
            is_square = false;
        if ((e % 4ULL) == 2ULL)
            has_exp2mod4 = true;
        /* τ increments by (e+1); compute modulo 4 */
        tau_mod4 = (tau_mod4 * ((e + 1ULL) % 4ULL)) & 3;
    }

    if (is_square) {                /* n is a perfect square  */
        return has_exp2mod4;         /* product not square iff some exponent ≡ 2 (mod 4) */
    } else {
        return (tau_mod4 == 2);      /* non‑square: product a square iff τ(n) ≡ 0 (mod 4) */
    }
}

/*  Static cache for sequence values.  */
static seq_t *cache = NULL;   /* array of a(k) values */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t  cache_sz  = 0;  /* number of values stored */

/*  Next integer to test.  */
static uint64_t next_candidate = 2;

/*  Grow the cache array to at least `min_cap` elements.  */
static void
grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < min_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/*  Ensure that the cache holds at least `need` terms.  */
static void
ensure_cache(idx_t need)
{
    while (cache_sz < need) {
        /* find the next member of the sequence */
        while (!is_non_square_product(next_candidate))
            ++next_candidate;

        /* store it */
        grow_cache(cache_sz + 1);
        cache[cache_sz] = (seq_t)next_candidate;
        ++cache_sz;
        ++next_candidate;
    }
}

/*  Return the n‑th term of the sequence (1‑based index).  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined; caller should use n >= 1 */
    ensure_cache(n);
    return cache[n - 1];
}
