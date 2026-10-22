/*
 * A361869  Let x_0, x_1, x_2, ... be the iterations of the arithmetic
 *          derivative A003415 starting with x_0 = n. a(n) is the greatest k
 *          such that x_0 > x_1 > ... > x_k.
 * https://oeis.org/A361869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of the arithmetic derivative A003415
 *            starting at n until the sequence stops being strictly decreasing
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the arithmetic derivative of a 64‑bit integer n.
   For n ≤ 1 the derivative is 0.  For a prime power p^e the derivative
   is e·p^{e−1}, and for a product the derivative follows from the
   product rule.  The implementation uses trial division and works
   for all n that fit into uint64_t. */
static seq_t arith_deriv_uint64(uint64_t n)
{
    if (n <= 1) return 0;

    uint64_t orig = n;
    seq_t result = 0;

    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            /* term = orig * exp / p */
            result += ((seq_t)orig * (seq_t)exp) / (seq_t)p;
        }
    }

    if (n > 1) {    /* remaining prime factor */
        result += ((seq_t)orig) / (seq_t)n;
    }

    return result;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t count = 0;

    while (1) {
        /* cur is guaranteed ≤ original n (uint64_t) before the
           algorithm terminates.  It is therefore safe to cast to
           uint64_t for the derivative calculation. */
        uint64_t cur64 = (uint64_t)cur;
        seq_t next = arith_deriv_uint64(cur64);

        if (next >= cur) break;
        cur = next;
        ++count;
    }

    return count;
}
