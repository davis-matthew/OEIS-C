/*
 * A392981  Expansion of e.g.f. 1 / (1 - Sum_{k>=1} Fibonacci(k+1)*x^k/k!).
 * https://oeis.org/A392981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = Sum_{k=1..n} binomial(n,k) * Fibonacci(k+1) *
 *            a(n-k) for n ≥ 1 (Fibonacci(0)=0, Fibonacci(1)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* return a(n) with external linkage */
seq_t a(idx_t n)
{
    /* static caches for previously computed a-values and Fibonacci numbers */
    static seq_t *a_cache = NULL;
    static idx_t   a_len   = 0;
    static seq_t *fib_cache = NULL;
    static idx_t   fib_len   = 0;

    /* extend a_cache to hold up to index n */
    if (n >= a_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        a_cache = tmp;
        for (idx_t i = a_len; i < new_len; ++i)
            a_cache[i] = 0;
        a_len = new_len;
    }

    /* extend fib_cache to hold up to index n+1 (need Fibonacci(k+1)) */
    if (n + 1 >= fib_len) {
        idx_t new_len = n + 2;          /* indices 0 .. n+1 */
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
        fib_cache = tmp;
        for (idx_t i = fib_len; i < new_len; ++i)
            fib_cache[i] = 0;
        fib_len = new_len;
    }

    /* initialize base cases */
    if (a_cache[0] == 0) a_cache[0] = 1;
    if (fib_cache[0] == 0) fib_cache[0] = 0;   /* not used directly */
    if (fib_cache[1] == 0) fib_cache[1] = 1;   /* F(1) */

    /* compute Fibonacci numbers up to n+1 */
    for (idx_t i = 2; i <= n + 1; ++i)
        if (fib_cache[i] == 0)
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];

    /* dynamic programming: compute a(m) for all 1 ≤ m ≤ n if not already known */
    for (idx_t m = 1; m <= n; ++m) {
        if (a_cache[m] != 0) continue;   /* already computed */
        seq_t sum = 0;
        seq_t binom = 1;                 /* binom(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* update binomial coefficient to binom(m,k) */
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;

            seq_t prod = binom * fib_cache[k + 1] * a_cache[m - k];
            sum += prod;
        }
        a_cache[m] = sum;
    }

    return a_cache[n];
}
