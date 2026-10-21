/*
 * A248509  Length of longest sequence of distinct nonzero squares summing to
 *          n, or 0 if no such sequence exists.
 * https://oeis.org/A248509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   length of longest sequence of distinct nonzero squares summing
 *            to n, or 0 if no such sequence exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dp_int[i] = maximum number of distinct squares whose sum equals i,
   or -1 if impossible.  dp_limit = current maximum n computed. */
static int *dp_int = NULL;
static unsigned long long dp_limit = 0ULL; /* idx_t is uint64_t */

/* Recompute dp table up to n.  Works for any n. */
static void recompute_dp(unsigned long long n)
{
    int *new_arr = (int *)malloc((n + 1) * sizeof(int));
    for (unsigned long long i = 0; i <= n; ++i)
        new_arr[i] = -1;
    new_arr[0] = 0;

    /* iterate over squares k^2 <= n */
    for (unsigned long long k = 1; ; ++k) {
        unsigned long long s = k * k;
        if (s > n)
            break;
        for (unsigned long long i = n; i >= s; --i) {
            int prev = new_arr[i - s];
            if (prev >= 0 && prev + 1 > new_arr[i])
                new_arr[i] = prev + 1;
        }
    }

    if (dp_int)
        free(dp_int);
    dp_int = new_arr;
    dp_limit = n;
}

/* OEIS sequence A248509: a(n) = length of longest sequence of distinct
   nonzero squares summing to n, or 0 if none exists. */
seq_t a(idx_t n)
{
    /* offset is 1, but function is safe for n == 0 as well */
    if (n == 0)
        return 0;

    if ((unsigned long long)n > dp_limit)
        recompute_dp((unsigned long long)n);

    int val = dp_int[(unsigned long long)n];
    return (val >= 0) ? (seq_t)val : 0;
}
