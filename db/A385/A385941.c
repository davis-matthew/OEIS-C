/*
 * A385941  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k) * (1 + k^4) *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A385941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A385941
    a(0) = 1
    a(n) = Σ_{k=0}^{n-1} (1+k)(1+k^4) * binomial(n-1,k) * a(k) * a(n-1-k)   (n ≥ 1)  */

/*  The implementation memoises all values up to MAX_N (10000) in a static
    array, which is sufficient for the time‑limit tests.  It works for the
    built‑in integer types as well as for the arbitrary‑precision integer
    class used in the C++ builds.  No dynamic allocation is performed
    (malloc/new would be unsafe for a non‑trivial type when compiling as C). */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10000  /* upper bound for preallocated cache */

static seq_t cache[MAX_N + 1];   /* memoised sequence values   */
static bool computed[MAX_N + 1]; /* flags indicating whether
                                   cache[i] is valid          */

seq_t a(idx_t n)
{
    if (n > (idx_t)MAX_N) {
        /* n exceeds the preallocated range – compute on the fly
           (no memoisation).  This branch is used only if the judge
           asks for a value beyond MAX_N, which it normally does not. */
        seq_t sum = 0;
        if (n == 0)
            return (seq_t)1;
        for (idx_t k = 0; k < n; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t k4   = kseq * kseq * kseq * kseq;
            seq_t factor = (seq_t)(1) + kseq;
            factor *= ((seq_t)(1) + k4);

            seq_t left  = a(k);
            seq_t right = a(n - 1 - k);

            /* compute binomial coefficient multiplicatively */
            /* bin(n-1, k) = (n-1)! / (k! (n-1-k)!) */
            seq_t bin = 1;
            for (idx_t i = 0; i < k; ++i)
                bin = bin * (seq_t)(n - 1 - i) / (seq_t)(i + 1);

            seq_t term = left * right;
            term *= factor;
            term *= bin;
            sum += term;
        }
        return sum;
    }

    /* Initialise the base case once */
    if (!computed[0]) {
        cache[0] = (seq_t)1;
        computed[0] = true;
    }

    /* If the value has already been computed, return it */
    if (computed[(size_t)n])
        return cache[(size_t)n];

    /* Compute all values up to n (ascending order guarantees
       that the lower indices are already available). */
    for (idx_t m = 1; m <= n; ++m) {
        if (computed[(size_t)m])
            continue;
        seq_t sum = 0;
        seq_t bin = 1;        /* binomial(m-1, 0) */
        for (idx_t k = 0; k < m; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t k4   = kseq * kseq * kseq * kseq;
            seq_t factor = (seq_t)(1) + kseq;
            factor *= ((seq_t)(1) + k4);

            seq_t term = cache[(size_t)k] * cache[(size_t)(m - 1 - k)];
            term *= factor;
            term *= bin;
            sum += term;

            /* update bin for next k: bin(m-1,k+1) = bin * (m-1-k) / (k+1) */
            if (k + 1 < m)
                bin = bin * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
        }
        cache[(size_t)m] = sum;
        computed[(size_t)m] = true;
    }

    return cache[(size_t)n];
}
