/*
 * A354998  Smallest divisor d of n for which A344005(d) = A344005(n), where
 *          A344005(n) is the smallest positive integer m such that n divides
 *          m*(m+1).
 * https://oeis.org/A354998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1508, u32 n <= 9350, u64 n <= 9350,
 *            u128 n <= 9350, big any n
 * Helpers:   A344005
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A354998:  a(n) = smallest divisor d of n with A344005(d) = A344005(n)
   where A344005(m) = smallest positive integer k such that m divides k*(k+1).
   Computes the value for any n ≥ 1, any idx_t or seq_t size. */

#include <stdint.h>   /* For uint64_t if needed */
#include "../utils/A344005.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* Undefined for 0, but safe fallback. */

    seq_t target = A344005(n);   /* A344005(n) */
    seq_t best = (seq_t)n;       /* Initialize with n itself */

    /* Iterate over all divisors of n, keeping the smallest that matches */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* Candidate d1 = i */
            seq_t d1 = (seq_t)i;
            if (A344005((idx_t)d1) == target && d1 < best) {
                best = d1;
            }
            /* Candidate d2 = n / i, avoid duplicate when i == n/i */
            if (i != n / i) {
                seq_t d2 = (seq_t)(n / i);
                if (A344005((idx_t)d2) == target && d2 < best) {
                    best = d2;
                }
            }
        }
    }
    return best;
}
