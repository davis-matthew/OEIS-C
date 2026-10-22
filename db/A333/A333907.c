/*
 * A333907  For n >= 1, a(n) = Sum_{k=1..n} prevfib(k) + nextfib(k) - 2*k,
 *          where prevfib(k) is the largest Fibonacci number < k, nextfib(k)
 *          is the smallest Fibonacci number > k.
 * https://oeis.org/A333907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (prevfib(k) + nextfib(k) - 2*k)
 * Range:     u8 n <= 64, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n ≥ 1, fib sequence defined with duplicate 1 at indices 1 and 2   */

#include <stddef.h>          /* size_t */
#include <stdint.h>          /* uint64_t */

/* Preallocate storage large enough for the intended range (≈10 000). */
#define MAXN   100000

static seq_t pref[MAXN + 1];   /* prefix sum array, pref[0] = 0          */
static idx_t pref_len = 0;     /* number of pref entries currently computed */

/* Fibonacci numbers table. A few elements are enough for n ≈10 000. */
static seq_t fib[200];
static idx_t fib_len = 0;      /* current number of Fibonacci numbers */

/* Initialise the Fibonacci table with the first few values.            */
static void init_fib(void)
{
    if (fib_len == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        fib[3] = (seq_t)2;          /* ensures at least two distinct Fibs   */
        fib_len = 4;
    }
}

/* Grow the Fibonacci table until the last element exceeds max_n.      */
static void extend_fib_upto(idx_t max_n)
{
    /* Ensure fib[fib_len - 1] > max_n. */
    while (fib_len == 0 || fib[fib_len - 1] <= max_n) {
        seq_t next = fib[fib_len - 1] + fib[fib_len - 2];
        fib[fib_len++] = next;
    }
}

/* Compute a(n).  The function is memoised: previously computed values are
   reused for any subsequent call (in any order).                         */
seq_t a(idx_t n)
{
    if (n == 0) {               /* offset is 1; a(0) is never used, return 0 */
        return (seq_t)0;
    }

    /* Initialise static data on first call. */
    if (pref_len == 0) {
        pref[0] = (seq_t)0;
        pref_len = 1;
        init_fib();
    }

    /* If we already have a(n), just return it. */
    if (n < pref_len) {
        return pref[(size_t)n];
    }

    /* Extend Fibonacci table to cover the new range. */
    extend_fib_upto(n);

    /* Compute terms for all k from pref_len to n. */
    idx_t i;                     /* index into fib for prevfib(k)   */
    for (i = 0; i < fib_len - 1 && fib[i] < pref_len; ++i) {}
    #define GET_PREV(k)  (fib[ (k <= pref_len) ? (idx_t)i : 0 ])
    /* But instead of this macro, use a small loop below for each k. */

    idx_t fib_idx = 0;           /* last fib < current k              */
    for (idx_t k = pref_len; k <= n; ++k) {
        /* Advance fib_idx to largest fib < k. */
        while (fib_idx + 1 < fib_len && fib[fib_idx + 1] < k) {
            ++fib_idx;
        }
        seq_t prev = fib[fib_idx];

        /* Find smallest fib > k. */
        idx_t j = fib_idx + 1;
        while (j < fib_len && fib[j] <= k) {
            ++j;
        }
        seq_t next = fib[j];

        seq_t term = prev + next - (seq_t)2 * (seq_t)k;
        pref[(size_t)k] = pref[(size_t)(k - 1)] + term;
    }
    pref_len = n + 1;

    return pref[(size_t)n];
}
