/*
 * A385652  Maximum frequency of gpf(k) for 2 <= k <= n, where gpf(k) =
 *          A006530(k) is the greatest prime factor of k.
 * https://oeis.org/A385652
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximum frequency of greatest prime factor of k for 2<=k<=n */
/* This implementation generates the gpf for each k in [2,n] using
 * the provided gpf() routine, keeps a frequency table (indexed by prime)
 * and returns the largest frequency found.  Complexity: O(n). */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;               /* values for n<2 are undefined */

    /* allocate a frequency array for primes up to n, zeroed below */
    size_t sz = (size_t)(n + 1);
    seq_t *freq = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!freq)
        return (seq_t)0;               /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        freq[i] = (seq_t)0;

    /* count occurrences of each greatest prime factor */
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t p = gpf((uint64_t)k);    /* p <= k <= n */
        if (p <= n)
            freq[(size_t)p]++;            /* seq_t supports ++ */
    }

    /* find the maximum frequency */
    seq_t maxf = (seq_t)1;                /* at least 1 for n>=2 */
    for (idx_t p = 2; p <= n; ++p) {
        if (freq[(size_t)p] > maxf)
            maxf = freq[(size_t)p];
    }

    free(freq);
    return maxf;
}
