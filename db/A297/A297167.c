/*
 * A297167  a(1) = 0, for n > 1, a(n) = -1 + the excess of n (A046660) + the
 *          index of the largest prime factor (A061395).
 * https://oeis.org/A297167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + (Ω(n) - ω(n)) + π(LPF(n))
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9569 (2026-11-01)
 * Helpers:   bigomega, largest_prime_factor, omega, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/omega.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t excess = bigomega(n) - (seq_t)omega(n);
    idx_t lp = (idx_t)largest_prime_factor(n);
    seq_t pidx = prime_pi(lp);
    return (excess + pidx) - 1;
}
