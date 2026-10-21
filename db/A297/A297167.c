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
 * Helpers:   bigomega, compute_pi, count_distinct_primes, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/gpf.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t excess = bigomega(n) - (seq_t)count_distinct_primes(n);
    idx_t lp = (idx_t)gpf(n);
    seq_t pidx = compute_pi(lp);
    return (excess + pidx) - 1;
}
