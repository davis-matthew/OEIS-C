/*
 * A346048  a(0) = ... = a(3) = 1; a(n) = Sum_{k=1..n-4} a(k) * a(n-k-4).
 * https://oeis.org/A346048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for 0 ≤ n ≤ 3 a(n) = Σ_{k=1}^{n-4} a(k) * a(n-k-4) for
 *            n ≥ 4 OEIS: A346048
 * Range:     u8 n <= 19, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define MAX_N 10000                      /* should cover all test values */

static seq_t cache[MAX_N + 1];
static size_t last_computed = (size_t)-1; /* no values computed yet */

seq_t a(idx_t n)
{
    /* avoid unsupported indices */
    if ((size_t)n > MAX_N) abort();

    /* if we have already computed up to n, return cached value */
    if (last_computed != (size_t)-1 && n <= last_computed)
        return cache[(size_t)n];

    /* determine from where to start computing next values */
    size_t start = (last_computed == (size_t)-1) ? 0 : last_computed + 1;
    if (start > (size_t)n) start = (size_t)n; /* safety for degenerate n */

    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;                     /* a(0)=a(1)=a(2)=a(3)=1 */
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= (idx_t)(i - 4); ++k) {
                sum += cache[(size_t)k] * cache[(size_t)(i - (size_t)k - 4)];
            }
            cache[i] = sum;
        }
    }

    last_computed = (size_t)n;   /* all values up to n are now ready */
    return cache[(size_t)n];
}
