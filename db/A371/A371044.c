/*
 * A371044  E.g.f. satisfies A(x) = 1 + x^3*exp(x*A(x)).
 * https://oeis.org/A371044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A371044:   a(n) = n! * Sum_{k=0..⌊n/3⌋} k^(n-3k) * binomial(n-3k+1,k)
/*                 / ((n-3k+1)*(n-3k)!)   ,  a(0)=1, a(1)=0, a(2)=0 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Cached factorials */
static seq_t *fact_cache = NULL;
static idx_t fact_cap = 0;

/* Ensure fact_cache[i] = i! for 0 <= i <= N */
static void ensure_factorial(idx_t N)
{
    if (N <= fact_cap)
        return;

    size_t new_cap = (size_t)N + 1;
    fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));

    /* Initialize fact_cache[0] if this is the first allocation */
    if (fact_cap == 0) {
        fact_cache[0] = (seq_t)1;
        fact_cap = 0;
    }

    for (idx_t i = fact_cap + 1; i <= N; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_cap = N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1 || n == 2)
        return (seq_t)0;

    /* Pre‑compute factorials up to n */
    ensure_factorial(n);
    const seq_t fact_n = fact_cache[n];
    const idx_t max_k = n / 3;

    seq_t sum = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* k^ (n - 3k)  */
        seq_t powk = big_pow((seq_t)k, (seq_t)(n - 3 * k));

        /* binomial(n - 3k + 1, k)  */
        seq_t bin = binomial(n - 3 * k + 1, k);

        /* product from (n - 3k + 2) to n = fact[n] / fact[n - 3k + 1] */
        idx_t denom_idx = n - 3 * k + 1;
        seq_t prod = fact_n / fact_cache[denom_idx];

        seq_t term = powk * bin * prod;
        sum += term;
    }
    return sum;
}
