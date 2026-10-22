/*
 * A383977  Sequence of successive merge positions when Fibonacci-sorting an
 *          infinite list.
 * https://oeis.org/A383977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(F(n+1)-1) = F(n); a(F(n)+k-1)=F(n)+a(k) for 0<k<F(n-1)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates Fibonacci numbers with F(1)=1,F(2)=2 and
 * computes a(n) recursively using the above relations.  The fib array is
 * cached statically and expanded only as far as needed.  All operations
 * are performed with the integer type seq_t supplied by the build. */
#include <stdlib.h>

static seq_t *fib = NULL;
static size_t fib_len = 0;   /* number of stored Fibonacci numbers (1-indexed) */
static size_t fib_cap = 0;   /* current allocated capacity */

/* enlarge the fib array to at least 'needed' elements (1-indexed) */
static void ensure_fib_len(size_t needed)
{
    if (needed <= fib_cap)
        return;
    size_t new_cap = fib_cap ? fib_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
    fib = tmp;
    fib_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined by OEIS, but harmless */

    /* Initialise Fibonacci table on first call */
    if (fib_len < 2) {
        ensure_fib_len(3);          /* we need indices 1 and 2 */
        fib[0] = (seq_t)0;          /* unused, keeps 1‑based indexing */
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)2;
        fib_len = 2;
    }

    seq_t Nplus1 = (seq_t)n + (seq_t)1;   /* compute n+1 in seq_t */

    /* grow fib until the last element is > Nplus1 */
    while (fib_len < fib_cap && fib[fib_len] <= Nplus1) {
        if (fib_len + 1 >= fib_cap)
            ensure_fib_len(fib_len + 2);
        fib[fib_len + 1] = fib[fib_len] + fib[fib_len - 1];
        fib_len++;
    }

    /* find m: largest index with fib[m] <= Nplus1 */
    size_t m = fib_len - 1;
    while (fib[m] > Nplus1)
        m--;

    if (Nplus1 == fib[m])           /* boundary position */
        return fib[m - 1];
    else {                           /* interior position */
        seq_t k = Nplus1 - fib[m];
        return fib[m] + a((idx_t)k);
    }
}
