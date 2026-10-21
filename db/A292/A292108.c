/*
 * A292108  Iterate the map k -> (sigma(k) + phi(k))/2 starting at n; a(n) is
 *          the number of steps to reach either a fixed point or a fraction,
 *          or a(n) = -1 if neither of these two events occurs.
 * https://oeis.org/A292108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 174, u32 n <= 269, u64 n <= 269,
 *            u128 n <= 269, big any n
 * Helpers:   calc_phi, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of steps of f(k)=(sigma(k)+phi(k))/2 until
 * termination (fixed point or fractional), with a(n)=0 if n is 1 or a prime.
 * For n>2 square or twice a square, the next step is fractional.
 */

/* required helper functions */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t steps = 0;
    seq_t cur   = (seq_t)n;

    /* fixed point: 1 or prime → 0 steps */
    if (cur == (seq_t)1 || is_prime((idx_t)cur))
        return steps;           /* 0 */

    while (1)
    {
        /* cur must fit in idx_t to call helper functions */
        if (cur > (seq_t)UINT64_MAX)
            return (seq_t)-1;    /* cannot continue, treat as divergence */

        idx_t cur_t = (idx_t)cur;
        seq_t sigma = sigma_of(cur_t);
        seq_t phi   = calc_phi(cur_t);
        seq_t sum   = sigma + phi;
        steps++;

        /* fraction iff sum is odd */
        if ((sum & 1u) != 0u)
            return steps;

        seq_t next = sum >> 1;   /* integer division by 2 */

        /* next is a fixed point? */
        if (next == (seq_t)1 || is_prime((idx_t)next))
            return steps;

        if (next > (seq_t)UINT64_MAX)
            return (seq_t)-1;    /* overflow before termination */

        cur = next;

        /* safety guard against an accidental infinite loop */
        if (steps > 1000000)
            return (seq_t)-1;
    }
}
