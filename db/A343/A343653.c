/*
 * A343653  Number of non-singleton pairwise coprime nonempty sets of divisors
 *          > 1 of n.
 * https://oeis.org/A343653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of all non‑singleton pairwise coprime nonempty sets of
 * divisors > 1 of n.  The implementation factors n, enumerates all
 * unordered partitions of the prime factor set, and counts the
 * corresponding divisor choices.  It runs in O(3^k) time where k is
 * the number of distinct primes of n (k ≤ 5 for n ≤ 10000). */

#include <stdint.h>
#include <stdlib.h>

/* Return the sequence term for index n.  idx_t is an unsigned 64‑bit
 * integer (uint64_t) and seq_t is an unsigned integer type of
 * unspecified width. */
seq_t a(idx_t n)
{
    if (n < 2)                /* no divisor > 1 exists */
        return 0;

    /* factor n: collect primes and their exponents */
    uint64_t primes[64];
    int       exps  [64];
    size_t    k = 0;
    uint64_t m = n;

    for (uint64_t p = 2; p*p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[k] = p;
            exps  [k] = e;
            ++k;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        primes[k] = m;
        exps  [k] = 1;
        ++k;
    }

    /* No non‑singleton pairwise coprime sets if fewer than two primes */
    if (k <= 1)
        return 0;

    /* DP over subsets of the k primes.
     * dp[mask] = number of ways to choose a set of pairwise coprime
     * divisors whose used primes are exactly mask.  Empty set is
     * represented by mask = 0 with dp[0] = 1. */
    size_t full = ((size_t)1) << k;
    seq_t* dp   = (seq_t*)malloc(full * sizeof(seq_t));
    if (!dp)
        return 0;            /* allocation failure → return 0 as fallback */

    dp[0] = 1;
    for (size_t mask = 1; mask < full; ++mask) {
        seq_t sum = 0;
        size_t lowest = mask & -mask;            /* lowest set bit */

        for (size_t sub = mask; sub; sub = (sub - 1) & mask) {
            if (!(sub & lowest))   /* enforce a canonical order of groups */
                continue;

            /* product of exponents for primes in this submask */
            seq_t prod = 1;
            size_t bits = sub;
            int idx = 0;
            while (bits) {
                if (bits & 1ULL)
                    prod *= (seq_t)exps[idx];
                bits >>= 1;
                ++idx;
            }

            sum += prod * dp[mask ^ sub];
        }
        dp[mask] = sum;
    }

    /* Sum over all non‑empty masks, subtract the single‑group
     * contributions to keep only sets of size ≥ 2. */
    seq_t ans = 0;
    for (size_t mask = 1; mask < full; ++mask) {
        /* product of exponents for mask (one‑group case) */
        seq_t prod = 1;
        size_t bits = mask;
        int idx = 0;
        while (bits) {
            if (bits & 1ULL)
                prod *= (seq_t)exps[idx];
            bits >>= 1;
            ++idx;
        }
        ans += dp[mask] - prod;
    }

    free(dp);
    return ans;
}
