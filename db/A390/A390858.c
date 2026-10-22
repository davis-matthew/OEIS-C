/*
 * A390858  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k+5,n-k) *
 *          Fibonacci(k+1).
 * https://oeis.org/A390858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k+5, n-k) *
 *            Fibonacci(k+1) Recurrence:
 *            a(n)=5*a(n-1)-11*a(n-2)+17*a(n-3)-14*a(n-4)+6*a(n-5)-a(n-6) with
 *            a(0)=1, a(1)=5, a(2)=14, a(3)=32, a(4)=77, a(5)=207
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t init[6] = {1,5,14,32,77,207};
static seq_t memo[10001];          /* supports n up to about 10000 */
static idx_t next_compute = 0;     /* first index still uncomputed */

/* Compute a(n) and cache intermediate results. */
seq_t a(idx_t n)
{
    /* Handle small n directly. */
    if (n < 6)
        return init[n];

    /* If we already have computed up to n, simply return the value. */
    if (next_compute <= n)
    {
        /* Lazily initialise the first six entries if not done yet. */
        if (next_compute < 6)
        {
            for (idx_t i = next_compute; i < 6 && i <= n; ++i)
                memo[i] = init[i];
            next_compute = 6;
        }

        /* Compute remaining values up to n. */
        for (idx_t i = next_compute; i <= n; ++i)
        {
            seq_t v;
            v = (seq_t)5 * memo[i-1];          /* 5*a(n-1)   */
            v -= (seq_t)11 * memo[i-2];        /* -11*a(n-2) */
            v += (seq_t)17 * memo[i-3];        /* +17*a(n-3) */
            v -= (seq_t)14 * memo[i-4];        /* -14*a(n-4) */
            v += (seq_t)6  * memo[i-5];        /* + 6*a(n-5) */
            v -= memo[i-6];                   /* - a(n-6)   */
            memo[i] = v;
        }
        next_compute = n + 1;
    }
    return memo[n];
}
