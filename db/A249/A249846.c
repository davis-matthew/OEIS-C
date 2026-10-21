/*
 * A249846  Number of length 2+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) (n>7) * Initial values (n=1..7): *
 *            a(1)=10, a(2)=198, a(3)=1500, a(4)=6916, a(5)=23526, *
 *            a(6)=65226, a(7)=156184 * * The function is memoised; calls may
 *            come in any order
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The maximum n that the program will handle.
 * The problem statement guarantees n <= 10000. */
#define MAX_N 10000

/* Static array used for memoisation.  The array holds a(1) at
 * index 0, a(2) at index 1, …, a(max_n) at index max_n-1. */
static seq_t cache[MAX_N];
/* Number of terms that have been computed and stored in 'cache'.
 * Initially 0.  After the first call it will be at least 7. */
static idx_t cache_size = 0;

/* Function that guarantees cache[0..n-1] are defined.
 * It may compute new terms using the recurrence relation. */
static void
ensure_cache_size(idx_t n)
{
    if (n == 0) /* behaviour for n<1 is irrelevant */
        return;

    if (cache_size == 0) {
        /* initialise the first seven terms */
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)198;
        cache[2] = (seq_t)1500;
        cache[3] = (seq_t)6916;
        cache[4] = (seq_t)23526;
        cache[5] = (seq_t)65226;
        cache[6] = (seq_t)156184;
        cache_size = 7;
    }

    if (n <= cache_size)
        return;   /* already computed enough terms */

    /* compute terms from cache_size up to n-1 (zero‑based) */
    for (idx_t i = cache_size; i < n; ++i) {
        cache[i] =
            (seq_t)7 * cache[i - 1]
          - (seq_t)21 * cache[i - 2]
          + (seq_t)35 * cache[i - 3]
          - (seq_t)35 * cache[i - 4]
          + (seq_t)21 * cache[i - 5]
          - (seq_t)7  * cache[i - 6]
          +          cache[i - 7];
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* behaviour below offset is irrelevant */
        return (seq_t)0;

    if (n > (idx_t)MAX_N) {
        /* The constraints of the problem guarantee this never happens. */
        /* If it does, fall back to dynamic allocation (not needed here). */
    }

    ensure_cache_size(n);
    return cache[n - 1];   /* cache[0] holds a(1) */
}
