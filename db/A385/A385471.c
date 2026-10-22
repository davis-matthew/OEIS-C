/*
 * A385471  Expansion of e.g.f. 1/(1 - 3 * arctanh(x)).
 * https://oeis.org/A385471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m odd ≤ n} binom(n, m) * 3 * (m-1)! * a(n-m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (binom(n, m) = n! / (m! (n-m)!) provided by factorial array) */

#include <stdint.h>
#include <stddef.h>

/* maximum n expected (adjust if needed) */
#define MAX_N 10000

/* factorials and cache arrays, initialized lazily */
static seq_t fact[MAX_N + 1];
static seq_t a_cache[MAX_N + 1];
static idx_t fact_sz = 0;
static idx_t cache_sz = 0;

/* precompute factorials up to MAX_N */
static void init_factorials(void)
{
    if (fact_sz > 0)
        return;  /* already done */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_sz = MAX_N + 1;
}

/* compute a(n) lazily and store in cache */
seq_t a(idx_t n)
{
    if (n > MAX_N)
        return (seq_t)0;  /* undefined for our bounds */

    init_factorials();

    /* extend cache up to n */
    for (idx_t i = cache_sz; i <= n; ++i)
    {
        if (i == 0)
        {
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t m = 1; m <= i; m += 2)  /* m odd */
        {
            /* binom(i, m) = fact[i] / (fact[m] * fact[i-m]) */
            seq_t denom = fact[m] * fact[i - m];
            seq_t binom = fact[i] / denom;

            seq_t term = binom;
            term *= a_cache[i - m];
            term *= (seq_t)3;
            term *= fact[m - 1];  /* (m-1)! */

            sum += term;
        }
        a_cache[i] = sum;
    }
    cache_sz = n + 1;
    return a_cache[n];
}
