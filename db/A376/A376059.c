/*
 * A376059  a(n) is the denominator of the sum S(n) defined in A376058.
 * https://oeis.org/A376059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n+1) = Fibonacci(n+1)*a(n)^2 + a(n), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *a_cache      = NULL;
static idx_t  a_cache_len  = 0;   /* number of filled entries */
static idx_t  a_cache_cap  = 0;   /* allocated capacity */

static seq_t *fib_cache    = NULL;
static idx_t  fib_len      = 0;   /* number of filled Fibonacci numbers */
static idx_t  fib_cap      = 0;   /* allocated capacity */

/* Ensure fib_cache holds Fibonacci numbers up to index n (inclusive). */
static void ensure_fib(idx_t n)
{
    if (fib_cache == NULL) {
        /* initialise with Fib(0)=0, Fib(1)=1 */
        idx_t init_cap = 2;
        fib_cap = init_cap;
        fib_len = 2;
        fib_cache = (seq_t*)malloc(sizeof(seq_t) * fib_cap);
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
    }

    /* grow the allocation if necessary */
    if (fib_cap <= n) {
        idx_t new_cap = fib_cap ? fib_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        for (idx_t i = 0; i < fib_len; ++i) tmp[i] = fib_cache[i];
        free(fib_cache);
        fib_cache = tmp;
        fib_cap = new_cap;
    }

    /* compute missing Fibonacci numbers */
    while (fib_len <= n) {
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        ++fib_len;
    }
}

/* Ensure a_cache holds values up to index n. */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {
        /* initialise a(0)=1 */
        a_cache_cap = 1;
        a_cache_len = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t) * a_cache_cap);
        a_cache[0] = (seq_t)1;
    }

    /* grow the allocation if necessary */
    if (a_cache_cap <= n) {
        idx_t new_cap = a_cache_cap ? a_cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        for (idx_t i = 0; i < a_cache_len; ++i) tmp[i] = a_cache[i];
        free(a_cache);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* ensure caches large enough */
    ensure_a_cache(n);
    ensure_fib(n);  /* need Fib(n) for a(n) */

    /* compute any missing values */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        /* skip i==0 because a(0) already stored */
        if (i == 0) continue;

        /* recurrence: a(i) = Fib(i) * a(i-1)^2 + a(i-1) */
        seq_t prev = a_cache[i - 1];
        seq_t sq   = prev * prev;
        seq_t prod = fib_cache[i] * sq;
        a_cache[i] = prod + prev;
    }

    /* update number of stored entries */
    a_cache_len = n + 1;

    return a_cache[n];
}
