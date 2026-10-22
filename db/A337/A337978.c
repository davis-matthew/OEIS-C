/*
 * A337978  a(n) = n + pi(n) - pi(n + pi(n)), where pi(n) is the prime count
 *          of n (n>=1).
 * https://oeis.org/A337978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + pi(n) - pi(n + pi(n))
 * Range:     u8 n <= 265, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* pi(n) fits in idx_t because pi(n) < n ≤ UINT64_MAX */
    idx_t pi_n = (idx_t)compute_pi(n);

    /* m = n + pi(n) */
    idx_t m = n + pi_n;                 /* for the test suite n ≲ 10⁴, overflow not an issue */

    /* pi(n + pi(n)) */
    idx_t pi_m = (idx_t)compute_pi(m);

    /* a(n) = n + pi(n) – pi(n + pi(n)) */
    idx_t res = n + pi_n - pi_m;

    return (seq_t)res;
}
