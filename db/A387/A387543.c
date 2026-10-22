/*
 * A387543  a(n) is the size of the largest subset of {1, 2, ..., n}
 *          containing n in which any two numbers share a prime factor.
 * https://oeis.org/A387543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the largest clique in the gcd‑graph that contains
 *            n. Based on the Erdős–Ahlswede theorem: For n = Π_i q_i^{k_i}
 *            choose j (1 ≤ j ≤ r) and take all numbers ≤ n that are divisible
 *            by at least one of {2q_1,…,2q_j, q_1···q_j}. The largest such
 *            set gives a(n). The following function comp...
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utility functions provided by the judge environment */
#include "../utils/gcd.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is at least 1 by the problem offset */
    if (n == 0) return 0;          /* safety, behaviour below offset */
    uint64_t N = (uint64_t)n;

    /* Simple trial division to get distinct prime divisors of N */
    uint64_t primesD[64];
    size_t r = 0;
    uint64_t m = N;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            primesD[r++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        primesD[r++] = m;
    }

    /* Special case: n == 1 has no prime factors */
    if (r == 0) return 1;

    /* Best size found so far */
    seq_t best = 0;

    /* For each j from 1 to r evaluate the set described in the theorem */
    for (size_t j = 1; j <= r; ++j) {
        /* Build list of multipliers:
             2*q_0, 2*q_1, ... ,2*q_{j-1},  q_0 * … * q_{j-1} */
        uint64_t mults[65];
        size_t mcnt = j + 1;
        for (size_t k = 0; k < j; ++k) {
            mults[k] = 2 * primesD[k];
        }
        uint64_t prod = 1;
        for (size_t k = 0; k < j; ++k) prod *= primesD[k];
        mults[j] = prod;

        /* Inclusion–exclusion over the mults array */
        seq_t count = 0;
        uint64_t combos = 1ULL << mcnt;
        for (uint64_t mask = 1; mask < combos; ++mask) {
            seq_t lcm_val = 1;
            int bits = __builtin_popcountll(mask);
            bool overflow = false;

            for (size_t idx = 0; idx < mcnt; ++idx) {
                if (!(mask & (1ULL << idx))) continue;

                seq_t a = (seq_t)mults[idx];
                seq_t g = gcd(lcm_val, a);

                seq_t div = lcm_val / g;
                /* Avoid overflow before multiplication */
                if (div > (seq_t)N / a) { overflow = true; break; }

                lcm_val = div * a;
                if (lcm_val > (seq_t)N) { overflow = true; break; }
            }

            if (overflow || lcm_val == 0 || lcm_val > (seq_t)N) continue;

            seq_t term = (seq_t)N / lcm_val;
            if (bits & 1) count += term;
            else count -= term;
        }

        if (count > best) best = count;
    }

    return best;
}
