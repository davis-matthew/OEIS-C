/*
 * A347050  Number of factorizations of n that are a twin (x*x) or have an
 *          alternating permutation.
 * https://oeis.org/A347050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335434(n) + A010052(n)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A010052(n) counts the unordered pairs a*b = n with 1 < a <= b   */
/*  A335434(n) counts the unordered factorizations of n having an alternating permutation. */

/* This implementation enumerates all weakly increasing factorizations of n
   and counts those that either have an alternating permutation or are a twin
   (two equal factors).  A factorization counts if there exists a permutation
   of its factors that contains no monotonic triple.  For a multiset this
   is equivalent to requiring that no factor value appears more than
   ceil(k/2) times, where k is the number of factors.  A two‑factor
   factorization is always counted, which explains the contribution
   from A010052.  The single‑factor factorization (n itself) is also
   counted.  The function works for any idx_t and seq_t defined by
   the build.  It is fully recursive; for n≤10000 the running time
   is well under the required limits. */

#include <stdint.h>
#include <stdbool.h>

static seq_t dfs(uint64_t rem, uint64_t min_f, uint64_t *factors, int depth)
{
    seq_t count = 0;

    if (rem == 1) {
        /* A factorization is complete.  Check eligibility. */
        if (depth == 0) /* should not happen for n >= 2 */
            return 0;

        /* Twin case: exactly two equal factors. */
        if (depth == 2 && factors[0] == factors[1])
            return (seq_t)1;

        /* Compute maximum multiplicity of any factor value. */
        int maxcnt = 1, curcnt = 1;
        for (int i = 1; i < depth; ++i) {
            if (factors[i] == factors[i - 1]) {
                ++curcnt;
            } else {
                if (curcnt > maxcnt)
                    maxcnt = curcnt;
                curcnt = 1;
            }
        }
        if (curcnt > maxcnt)
            maxcnt = curcnt;

        /* Alternating condition equivalent to no value appearing more than ceil(k/2). */
        if (maxcnt <= (depth + 1) / 2)
            return (seq_t)1;

        return 0;
    }

    /* Enumerate divisors f >= min_f such that f*f <= rem. */
    for (uint64_t f = min_f; f * f <= rem; ++f) {
        if ((rem % f) == 0) {
            factors[depth] = f;
            count += dfs(rem / f, f, factors, depth + 1);
        }
    }

    /* Use rem itself as the last factor (sequence length increased by 1). */
    if (rem >= min_f) {
        factors[depth] = rem;
        count += dfs(1, rem, factors, depth + 1);
    }

    return count;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;  /* Special case: factorization of 1 itself. */

    /* Allocate enough space for the deepest recursion (worst case: all 2s). */
    uint64_t factors[64];   /* 2^64 > 10^18, ample for n up to very large limits. */
    return dfs((uint64_t)n, 2, factors, 0);
}
