/*
 * A337979  Define a map f(n):= n-> n + pi(n) - pi(n + pi(n)), where pi(n) is
 *          the prime count of n (n>=1). a(n) is the number of steps for n to
 *          reach 1 under repeated iteration of f.
 * https://oeis.org/A337979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 615, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337979: number of iterations to reach 1 under
   f(n) = n + π(n) – π(n + π(n)).
   The function iteratively applies f until the value becomes 1
   and counts how many steps were needed. */
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1u) return 0;

    idx_t cur = n;
    idx_t steps = 0;

    while (cur > 1u) {
        seq_t pi_cur = compute_pi(cur);                 /* π(cur)   */
        idx_t next  = cur + (idx_t)pi_cur;              /* cur + π(cur) */
        seq_t pi_next = compute_pi(next);               /* π(cur + π(cur)) */

        /* Because π(next) > π(cur) for cur > 1, the difference is positive. */
        idx_t delta = (idx_t)(pi_next - pi_cur);
        cur -= delta;
        ++steps;
    }

    return (seq_t)steps;
}
