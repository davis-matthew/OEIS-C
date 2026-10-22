/*
 * A365737  Length of the longest subsequence of 1,...,n on which the Euler
 *          totient function phi A000010 is nonincreasing.
 * https://oeis.org/A365737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A365737:  a(n) = length of the longest subsequence of 1,…,n on
    which Euler's totient function φ is non‑increasing.  The implementation
    uses a sieve to compute φ(1)…φ(n) and a patience‑sorting algorithm
    that finds the longest non‑decreasing subsequence of −φ, which is
    equivalent to the desired longest non‑increasing subsequence.  The
    search is O(n log n) and works for n up to about 10 000 in well under
    two seconds. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for totient values.  Index 0 is unused. */
    idx_t *phi = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!phi) return (seq_t)0;

    /* Initialise φ(m) = m. */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;

    /* Classic sieve to compute φ(1)…φ(n). */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {                 /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }

    /* Patience sorting to find the longest non‑increasing subsequence
       of φ.  We transform φ to w = -φ and find the longest
       non‑decreasing subsequence of w.  For non‑decreasing we need an
       "upper_bound" search so that equal values extend the sequence. */
    int64_t *tails = (int64_t *)malloc((size_t)n * sizeof(int64_t));
    if (!tails) { free(phi); return (seq_t)0; }

    size_t tail_len = 0;
    idx_t result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        int64_t w = -(int64_t)phi[i];

        /* Find the first position > w (upper_bound). */
        size_t lo = 0, hi = tail_len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (tails[mid] <= w)
                lo = mid + 1;
            else
                hi = mid;
        }

        if (lo == tail_len) {
            tails[tail_len++] = w;
        } else {
            tails[lo] = w;
        }
        result = (idx_t)tail_len;
    }

    free(tails);
    free(phi);
    return (seq_t)result;
}
