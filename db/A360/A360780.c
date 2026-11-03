/*
 * A360780  Least refactorable number > n.
 * https://oeis.org/A360780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least refactorable number > n (a number m such that m % τ(m) == 0) */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_divisors.h"

/* Return true if m is refactorable: m divisible by its divisor count τ(m). */
static inline bool is_refactorable(seq_t m) {
    /* a number of zero divisors is not defined; 0 is not refactorable */
    if (m == (seq_t)0) return false;
    uint64_t tau = num_divisors((uint64_t)m);
    return ((m % (seq_t)tau) == (seq_t)0);
}

/* Return the least refactorable number strictly greater than n. */
seq_t a(idx_t n) {
    seq_t candidate = (seq_t)(n + (idx_t)1);
    while (!is_refactorable(candidate)) {
        candidate++;
    }
    return candidate;
}
