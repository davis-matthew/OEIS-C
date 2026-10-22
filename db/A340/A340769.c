/*
 * A340769  The third-smallest divisor of n-th square number, n>1.
 * https://oeis.org/A340769
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = third-smallest divisor of n² (n>1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/find_spf.h"

/* OEIS A340769: third-smallest divisor of n², with n > 1. */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t p1 = find_spf(nn);

    /* Remove all powers of p1 from n. */
    uint64_t m = nn / p1;
    while (m % p1 == 0ULL) {
        m /= p1;
    }

    /* Candidate values: p1² and, if it exists, the second smallest prime factor p2. */
    seq_t candidate1 = (seq_t)p1 * (seq_t)p1;
    if (m == 1ULL) {
        /* n is a prime power: only divisor >1 come from p1. */
        return candidate1;
    } else {
        uint64_t p2 = find_spf(m);
        seq_t candidate2 = (seq_t)p2;
        return candidate1 < candidate2 ? candidate1 : candidate2;
    }
}
