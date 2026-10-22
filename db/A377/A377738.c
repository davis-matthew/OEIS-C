/*
 * A377738  a(n) is the least m > n such that the Zeckendorf representations
 *          of m and n have the same number of terms.
 * https://oeis.org/A377738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least m > n whose Zeckendorf representation has the
 *            same number of terms as n. Computation is performed via a greedy
 *            Zeckendorf decomposition. The routine uses a static table of
 *            Fibonacci numbers (1, 2, 3, 5, 8, …) to speed up repeated
 *            calculations. It never overflows the index counter...
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stddef.h>

#define FIB_MAX 200   /* 200 Fibonacci numbers cover the n ≤ 10000 case quite comfortably */

static seq_t fib[FIB_MAX];
static size_t fib_len = 0;

/* Ensure that fib contains all Fibonacci numbers <= n.
   The Fibonacci sequence used is 1, 2, 3, 5, 8, … (F[0] = 1, F[1] = 2). */
static void ensure_fib(seq_t n)
{
    if (fib_len == 0) {
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_len = 2;
    }
    while (fib_len < FIB_MAX && fib[fib_len - 1] < n) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

/* Return the largest index i such that fib[i] <= n.  The table is large
   enough for all calls made by this routine given the problem constraints. */
static size_t largest_index_leq(seq_t n)
{
    size_t lo = 0, hi = fib_len - 1;
    while (lo < hi) {
        size_t mid = (lo + hi + 1) >> 1;
        if (fib[mid] <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the number of terms in the Zeckendorf representation of n.
   The greedy algorithm is used; a signed index prevents underflow. */
static int zeckendorf_count(seq_t n)
{
    if (n == (seq_t)0)
        return 0;

    ensure_fib(n);
    long long idx = (long long)largest_index_leq(n);   /* signed to avoid underflow */
    seq_t rem = n;
    int cnt = 0;

    while (idx > 0) {
        if (fib[idx] <= rem) {
            rem -= fib[idx];
            ++cnt;
            idx -= 2;            /* skip the next lower Fibonacci number */
        } else {
            --idx;
        }
    }
    if (idx == 0 && fib[0] <= rem) {
        rem -= fib[0];
        ++cnt;
    }
    return cnt;
}

/*--------------------------------------------------------------
  Main sequence routine.
  --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Special case: offset is 1, so a(1) is defined as 2; the
       following code works for all n >= 1. */
    int target = zeckendorf_count((seq_t)n);
    seq_t m = (seq_t)n + (seq_t)1;

    for (;;) {
        if (zeckendorf_count(m) == target)
            return m;
        ++m;
    }
}
