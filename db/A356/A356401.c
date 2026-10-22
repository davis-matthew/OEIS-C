/*
 * A356401  a(n) = n! * Sum_{k=1..n} Sum_{d|k} (-1)^(d+1)/(d * (k/d)!).
 * https://oeis.org/A356401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} Sum_{d|k} (-1)^(d+1)/(d * (k/d)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for factorials */
static seq_t *fact = NULL;
static size_t fact_cap = 0;
static idx_t fact_last = 0;

/* Ensure factorials up to n are available */
static void ensure_factorial(idx_t n)
{
    if (n <= fact_last) return;

    size_t new_cap = fact_cap > 0 ? fact_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    fact_cap = new_cap;

    if (fact_last == 0) fact[0] = (seq_t)1;

    for (idx_t i = fact_last + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    fact_last = n;
}

/* Static storage for already computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0;

/* Ensure cache array can hold values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last) return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    cache_last = n;
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];   /* value already computed */

    ensure_factorial(n);

    seq_t pos = 0, neg = 0;
    seq_t fn = fact[n];

    for (idx_t d = 1; d <= n; ++d) {
        idx_t max_m = n / d;
        bool positive = (d & 1);            /* d odd => sign +, even => sign - */

        for (idx_t m = 1; m <= max_m; ++m) {
            seq_t tmp   = fn / fact[m];           /* n! / m!  (integer) */
            seq_t term  = tmp / (seq_t)d;        /* term = n! / (d * m!) (integer) */

            if (positive)
                pos += term;
            else
                neg += term;
        }
    }

    seq_t res = pos - neg;    /* pos >= neg for all n >= 1 */
    cache[n] = res;
    return res;
}
