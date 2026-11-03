/*
 * A357409  a(n) is the maximum number of positive numbers in a set of n
 *          consecutive positive or negative odd numbers such that the number
 *          of pairs that add to a power of 2 is maximal.
 * https://oeis.org/A357409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS A357409: a(n) is the maximum number of positive odd numbers
    in any set of n consecutive odd integers that obtains the
    maximum possible number of unordered pairs whose sum is a power of 2.
    Brute‑force search over the odd starting value is
    sufficient for n ≤ 10 000 (and in practice for larger n).
    Complexity is O(n log n) with a very small constant. */

#include <stdbool.h>
#include <stdint.h>

/* Count of i<j with i,j∈[0,n−1] such that i+j = S.
   We use unsigned arithmetic because all values are small. */
static inline seq_t pair_count_for_sum(idx_t n, idx_t S)
{
    if (S < 1)
        return (seq_t)0;

    /* i ranges from max(0, S−(n−1)) to floor((S−1)/2) capped at n−2. */
    idx_t low  = (S > n - 1) ? (S - (n - 1)) : 0;
    idx_t high = (S - 1) / 2;
    if (high > n - 2)
        high = n - 2;

    if (high >= low)
        return (seq_t)(high - low + 1);
    return (seq_t)0;
}

/* Number of pairs in the sequence a, a+2, …, a+2(n−1).  */
static inline seq_t pair_count_for_start(idx_t n, long long a)
{
    long long smax = 2LL * (long long)n - 3;
    if (smax < 1)
        return (seq_t)0;                     /* no possible i+j */

    long long bound = a + smax;                /* largest possible power of two */
    if (bound < 1)
        return (seq_t)0;                      /* no power of two reachable */

    seq_t total = (seq_t)0;
    /* iterate over powers of two 2^k for k ≥ 0 (so sums 2,4,8,…) */
    for (long long pow = 1; pow <= bound; pow <<= 1) {
        long long S = pow - a;                 /* a + i + j = pow/2  →  i+j = S  */
        if (S >= 1 && S <= smax)
            total += pair_count_for_sum(n, (idx_t)S);
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* search range for the odd starting value */
    long long a_high = 4LL * (long long)n;
    if ((a_high & 1LL) == 0LL)
        a_high -= 1;                 /* make it odd (3, 7, 11, …) */

    long long a_low  = -(4LL * (long long)n);
    if ((a_low & 1LL) == 0LL)
        a_low -= 1;                  /* make it odd (−5, −9, …) */

    seq_t   bestPairs = 0;
    long long bestA   = a_low;
    bool     first    = true;

    for (long long a = a_high; a >= a_low; a -= 2) {
        seq_t pairs = pair_count_for_start(n, a);

        if (first || pairs > bestPairs || (pairs == bestPairs && a > bestA)) {
            bestPairs = pairs;
            bestA     = a;
            first     = false;
        }
    }

    /* Count positive terms in the chosen sequence. */
    long long neg = 0;
    if (bestA <= -1)                         /* negative start gives some negatives */
        neg = ((-bestA) / 2) + 1;            /* number of odd numbers ≤ 0 in the set */

    long long pos = (long long)n - neg;       /* remaining terms are positive */

    return (seq_t)pos;
}
