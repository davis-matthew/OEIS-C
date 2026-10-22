/*
 * A350834  Number of ways to tile an n X n right triangle with squares and
 *          dominoes, where vertical dominoes are only allowed in the largest
 *          vertical column.
 * https://oeis.org/A350834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A350834
    Recurrence:  a(0) = 1,  a(1) = 1
                a(n) = Fibonacci(n+1)*a(n-1) + Fibonacci(n)*Fibonacci(n-1)*a(n-2)   (n ≥ 2)
*/

#include <stdlib.h>
#include <stdint.h>

/* ---- Fibonacci cache ---- */
static seq_t *fib_cache = NULL;   /* array of Fibonacci numbers F[0..] */
static size_t fib_cap   = 0;      /* current capacity of fib_cache */
static idx_t  fib_last  = (idx_t)-1;  /* last index that has been initialized */

/* Ensure fib_cache contains Fibonacci numbers up to index n (inclusive). */
static void ensure_fib(idx_t n)
{
    if (fib_cap > (size_t)n + 1U)          /* already enough space */
        return;

    size_t new_cap = (size_t)n + 2;        /* need indices 0..n  -> size n+2 */
    seq_t *tmp = (seq_t*)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failed */
    fib_cache = tmp;
    fib_cap   = new_cap;

    /* Initialize the first two Fibonacci numbers if this is the first time. */
    if (fib_last == (idx_t)-1) {
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_last = 1;
    }

    for (idx_t i = fib_last + 1; i <= n; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_last = n;
}

/* ---- A350834 cache ---- */
static seq_t *a_cache = NULL;   /* array of a(n) values */
static size_t a_cap   = 0;
static idx_t  a_last  = (idx_t)-1;  /* last index that has been computed */

/* Ensure a_cache contains a(0) .. a(n). */
static void ensure_a(idx_t n)
{
    if (a_cap > (size_t)n + 1U)          /* already enough space */
        return;

    size_t new_cap = (size_t)n + 1;      /* indices 0..n  -> size n+1 */
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failed */
    a_cache = tmp;
    a_cap   = new_cap;

    /* Initialize base cases if this is the first time. */
    if (a_last == (idx_t)-1) {
        a_cache[0] = 1;
        if (n >= 1) a_cache[1] = 1;
        a_last = (n >= 1) ? 1 : 0;
    }

    /* Make sure we have Fibonacci numbers up to index n+1 (needed for recurrence). */
    ensure_fib(n + 1);

    for (idx_t k = a_last + 1; k <= n; ++k) {
        seq_t t1 = fib_cache[k+1] * a_cache[k-1];
        seq_t t2 = fib_cache[k] * fib_cache[k-1] * a_cache[k-2];
        a_cache[k] = t1 + t2;
    }
    a_last = n;
}

/* ---- Public function ---- */
seq_t a(idx_t n)
{
    /* n is unsigned, do not worry about negative values */
    ensure_a(n);
    return a_cache[n];
}
