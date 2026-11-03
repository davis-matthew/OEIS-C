/*
 * A354998  Smallest divisor d of n for which A344005(d) = A344005(n), where
 *          A344005(n) is the smallest positive integer m such that n divides
 *          m*(m+1).
 * https://oeis.org/A354998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1508, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 9350 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A354998:  a(n) = smallest divisor d of n with A344005(d) = A344005(n)
   where A344005(m) = smallest positive integer k such that m divides k*(k+1).
   Computes the value for any n ≥ 1, any idx_t or seq_t size. */

#include <stdint.h>   /* For uint64_t if needed */

/* Compute A344005 for a given n (uint64_t). */
static seq_t A344005(idx_t n) {
    seq_t sn = (seq_t)n;   /* Convert to seq_t for arithmetic */
    seq_t k = 1;
    /* Search k until sn | k*(k+1).  This always terminates with k ≤ n-1. */
    for (;;) {
        seq_t prod = k * (k + 1);
        if (prod % sn == 0) {
            return k;
        }
        ++k;
    }
}

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
