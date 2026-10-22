/*
 * A364194  a(n) = Sum_{k=1..n} k^3*sigma(k), where sigma is A000203.
 * https://oeis.org/A364194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A364194
    a(n) = Sum_{k=1..n} k^3 * sigma(k)   (sigma is the sum of divisors) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* unless n == 0 we ignore the value; offset is 1 */
    if (n == 0) return 0;

    /* we recompute from scratch every time – O(n log n),
       which is fast enough for n up to ~10000. */
    static seq_t *sigma = NULL;
    static seq_t *cumsum = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {
        idx_t i, j, k;
        /* allocate needed space */
        sigma = (seq_t *)realloc(sigma, (n + 1) * sizeof(seq_t));
        if (!sigma) return 0;              /* allocation failure */
        cumsum = (seq_t *)realloc(cumsum, (n + 1) * sizeof(seq_t));
        if (!cumsum) return 0;

        /* initialise all entries to zero (must work for arbitrary‐precision types) */
        for (i = 0; i <= n; ++i) sigma[i] = 0;

        /* compute sigma(k) for 1 .. n */
        for (i = 1; i <= n; ++i) {
            seq_t ii = (seq_t)i;
            for (j = i; j <= n; j += i) {
                sigma[j] += ii;
            }
        }

        /* compute cumulative sums a(k) = Σ_{t=1..k} t^3 * sigma(t) */
        cumsum[0] = 0;
        for (k = 1; k <= n; ++k) {
            seq_t kk = (seq_t)k;
            seq_t term = kk * kk * kk * sigma[k];
            cumsum[k] = cumsum[k - 1] + term;
        }
        max_n = n;
    }

    return cumsum[n];
}
