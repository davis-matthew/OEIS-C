/*
 * A367293  Main diagonal of the extended Wythoff array (A287870).
 * https://oeis.org/A367293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Fibonacci(n-1) + Fibonacci(n)*floor(((1 + sqrt(5))*(1 +
 *            n))/2), for n > 0, a(0) = 0. Uses the Fibonacci cache and the
 *            utility floor_sqrt
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* ---------- Fibonacci cache ---------- */
static seq_t *fib_cache = NULL;      /* fib_cache[0] = 0, fib_cache[1] = 1 */
static size_t fib_len   = 0;         /* number of entries stored (>= 2 when non‑NULL) */
static size_t fib_cap   = 0;         /* allocated capacity of fib_cache */

/* Ensure fib_cache holds Fibonacci numbers up to index n inclusive. */
static void ensure_fib(idx_t n)
{
    size_t need = (size_t)n + 1;          /* need indices 0 … n  */
    if (fib_cache == NULL)                /* first allocation */
    {
        size_t init_cap = (need < 2 ? 2 : need);
        fib_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!fib_cache) exit(1);
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
        fib_cap = init_cap;
    }

    /* Grow capacity if necessary */
    if (need > fib_cap)
    {
        fib_cap = need * 2;               /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(fib_cache, fib_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fib_cache = tmp;
    }

    /* Compute missing Fibonacci values */
    if (need > fib_len)
    {
        for (idx_t i = (idx_t)fib_len; i <= n; ++i)
        {
            fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
        }
        fib_len = need;
    }
}

/* ---------- Sequence a(n) ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Prepare Fibonacci numbers */
    ensure_fib(n);
    seq_t Fn   = fib_cache[n];   /* Fibonacci(n)   */
    seq_t Fn_1 = fib_cache[n - 1];/* Fibonacci(n-1) */

    /* Compute floor((1+sqrt(5))*(n+1)/2)  =  floor(( (n+1)*sqrt(5) + (n+1)) / 2)   */
    seq_t k          = (seq_t)(n + 1);              /* k = n+1 */
    seq_t sq          = (seq_t)5 * k * k;           /* 5*k^2  */
    seq_t sqrt_term   = floor_sqrt(sq);            /* floor(k*sqrt(5)) */
    seq_t phi_floor   = (k + sqrt_term) / 2;        /* floor(phi*k) */

    /* Final formula */
    seq_t part1 = (seq_t)n * Fn_1;
    seq_t part2 = Fn * phi_floor;
    return part1 + part2;
}
