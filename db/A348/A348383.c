/*
 * A348383  Number of factorizations of n that are either separable (have an
 *          anti-run permutation) or are a twin (x*x).
 * https://oeis.org/A348383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335434(n) + A010052(n) A335434(n) : number of separable
 *            weak‑increasing factorizations of n A010052(n) : 1 if n is a
 *            perfect square, otherwise 0. (Separability test: a multiset is
 *            separable iff 2*maxfreq ≤ k+1, where k is the number of parts
 *            and maxfreq the largest multiplicity.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for idx_t (returns floor(sqrt(n))) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    return lo;
}

/* Recursive enumeration of all weakly increasing factorizations of rem.
   parts[depth…] holds the current sequence of factors.
   count accumulates the number of separable factorizations. */
static void dfs(idx_t rem, idx_t prev, size_t depth,
                idx_t *parts, seq_t *count)
{
    idx_t limit = isqrt_u64(rem);
    for (idx_t f = prev; f <= limit; ++f) {
        if (rem % f == 0) {
            parts[depth] = f;
            dfs(rem / f, f, depth + 1, parts, count);
        }
    }
    /* Handle the remaining factor rem itself (completed factorization). */
    if (rem >= prev) {
        parts[depth] = rem;
        size_t k = depth + 1;   /* number of parts */

        /* Count multiplicities of equal consecutive parts. */
        size_t maxfreq = 1, cur = 1;
        for (size_t i = 1; i < k; ++i) {
            if (parts[i] == parts[i - 1]) {
                ++cur;
            } else {
                if (cur > maxfreq) maxfreq = cur;
                cur = 1;
            }
        }
        if (cur > maxfreq) maxfreq = cur;

        /* Separability test: 2*maxfreq ≤ k+1. */
        if (2 * maxfreq <= k + 1) {
            ++(*count);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) {
        /* By definition a(1) = 1. */
        return (seq_t)1;
    }

    static idx_t parts[64];          /* buffer for current factorization */
    seq_t separable = 0;

    /* Enumerate all factorizations of n (>1). */
    dfs(n, 2, 0, parts, &separable);

    /* Add 1 for the twin factorization if n is a perfect square. */
    idx_t r = isqrt_u64(n);
    if (r * r == n) {
        ++separable;
    }

    return separable;
}
