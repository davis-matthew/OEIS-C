/*
 * A331131  a(n) = Sum_{i=1..n} d(i)*d_3(i+1), where d(n) = A000005(n) and
 *          d_3(n) = A007425(n).
 * https://oeis.org/A331131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(i) * d_3(i+1) where d(i) is the number of
 *            divisors of i (A000005) and d_3(i) = Sum_{d|i} d(d) (A007425).
 *            The implementation keeps a cached prefix sum so that any call
 *            a(n) for n ≥ 1 is answered in O(1) after an initial O(n log n)
 *            sieve which is recomputed only when a larger...
 * Range:     u8 n <= 14, u16 n <= 566, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1; values for n<1 are irrelevant */

    static idx_t current_limit = 0;     /* largest n seen so far (i.e. current n+1) */
    static uint64_t *divCount   = NULL; /* d(i) for i up to current_limit */
    static uint64_t *div3       = NULL; /* d_3(i) for i up to current_limit */
    static seq_t   *pref        = NULL; /* prefix sums, pref[i] = Σ_{k=1..i} d(k)*d_3(k+1) */

    /* Desired maximum index is (n+1) because we need d_3(n+1). */
    idx_t needed = n + 1;

    if (needed > current_limit)
    {
        /* Free previous storage if it exists. */
        if (divCount) free(divCount);
        if (div3)     free(div3);
        if (pref)     free(pref);

        current_limit = needed;

        /* Allocate storage:
           indices 0 .. current_limit are needed for divCount and div3.
           pref only needs to store 0 .. n but we keep the same size for simplicity.
        */
        divCount = (uint64_t*)calloc((size_t)current_limit + 1, sizeof(uint64_t));
        div3     = (uint64_t*)calloc((size_t)current_limit + 1, sizeof(uint64_t));
        pref     = (seq_t*)   calloc((size_t)current_limit + 1, sizeof(seq_t));

        /* -------- compute d(i) -------- */
        for (idx_t i = 1; i <= current_limit; ++i)
            for (idx_t j = i; j <= current_limit; j += i)
                ++divCount[j];

        /* -------- compute d_3(i) --------
           d_3(m) = Σ_{d|m} d(d) = Σ_{d|m} divCount[d].
        */
        for (idx_t d = 1; d <= current_limit; ++d)
            for (idx_t m = d; m <= current_limit; m += d)
                div3[m] += divCount[d];

        /* -------- compute prefix sums --------
           pref[0] = 0.  pref[i] = Σ_{k=1..i} d(k)*d_3(k+1) for i ≤ n.
        */
        pref[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
        {
            seq_t prod = (seq_t)divCount[i] * (seq_t)div3[i + 1];
            pref[i] = pref[i - 1] + prod;
        }
    }

    /* The requested value is already cached. */
    return pref[n];
}
