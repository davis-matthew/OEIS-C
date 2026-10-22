/*
 * A307461  Number of longest paths in the stacked book graph S_{n+1} square
 *          P_n.
 * https://oeis.org/A307461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 8 a(3) = 102 For n >= 4: if n even: a(n) = n *
 *            (n-1) * (((n/2)-1)!)^2 * n! if n odd : a(n) = 3 * (2*n-1) *
 *            (((n-1)/2)!)^2 * n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;      /* factorial cache */
static size_t fact_cap   = 0;         /* allocated size of cache   */
static idx_t  fact_size  = 0;         /* number of entries in cache */

/* Ensure that fact_cache[i] is computed for all i <= n */
static void
ensure_factorial(idx_t n)
{
    if (n < (idx_t)fact_size) return;

    if (fact_cache == NULL) {
        fact_cap  = 1;
        fact_cache = (seq_t*)malloc(sizeof(seq_t) * fact_cap);
        if (!fact_cache) exit(1);          /* unable to allocate */
        fact_cache[0] = 1;                 /* 0! = 1 */
        fact_size   = 1;
    }
    if ((idx_t)(fact_cap) <= n) {
        size_t new_cap = fact_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(fact_cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);                 /* unable to reallocate */
        fact_cache = tmp;
        fact_cap   = new_cap;
    }
    for (idx_t i = fact_size; i <= n; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_size = n + 1;
}

/* Return factorial of n, using the cache */
static seq_t
fact(idx_t n)
{
    ensure_factorial(n);
    return fact_cache[n];
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 8;
    if (n == 3) return 102;

    seq_t result;

    if ((n & 1) == 0) {                 /* even n */
        idx_t half = n / 2;
        seq_t f_half = fact(half - 1);          /* ((n/2)-1)! */
        seq_t f_half_sq = f_half * f_half;
        seq_t f_n = fact(n);
        result = (seq_t)n * (seq_t)(n - 1);
        result = result * f_half_sq;
        result = result * f_n;
    } else {                            /* odd n */
        idx_t half = (n - 1) / 2;
        seq_t f_half = fact(half);                /* ((n-1)/2)! */
        seq_t f_half_sq = f_half * f_half;
        seq_t f_n = fact(n);
        result = 3 * (seq_t)(2 * n - 1);
        result = result * f_half_sq;
        result = result * f_n;
    }
    return result;
}
