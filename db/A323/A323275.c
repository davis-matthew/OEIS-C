/*
 * A323275  Let f(p, q) denote the pair (p + q, wt(p) + wt(q)); a(n) is
 *          obtained by iterating f starting at (n, 1) until p/q is an integer
 *          (and then a(n) is that integer), or if no integer is ever reached
 *          then a(n) = -1. (Here wt is binary weight, A000120.)
 * https://oeis.org/A323275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A323275:  a(n) is the integer p/q reached by iterating
 * f(p,q) = (p+q, wt(p)+wt(q)) starting from (n,1) until
 * p/q is an integer, or -1 if no integer arises.  wt(x) is
 * the binary weight of x (A000120).  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* popcount of a seq_t value: works for any width by processing
 * 8‑bit chunks and using the builtin popcount for 8‑bit values. */
static inline seq_t popcount_seq(seq_t x)
{
    seq_t count = (seq_t)0;
    const seq_t mask8 = (seq_t)0xFFU;
    while (x != (seq_t)0) {
        uint8_t part = (uint8_t)(x & mask8);
        count += (seq_t)__builtin_popcount((unsigned)part);
        x >>= 8;
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n;
    seq_t q = (seq_t)1;

    /* Upper bound on iterations; the sequence stabilises
     * quickly for the ranges tested (≈10^4). */
    const unsigned MAX_STEPS = 200000;

    for (unsigned step = 0; step < MAX_STEPS; ++step)
    {
        /* Compute the next pair according to f. */
        seq_t wp = popcount_seq(p);
        seq_t wq = popcount_seq(q);
        seq_t pn = p + q;
        seq_t qn = wp + wq;

        p = pn;
        q = qn;

        /* After the first application of f, check if the
         * ratio is an integer.  q is always > 0. */
        if (p % q == (seq_t)0)
            return p / q;
    }

    /* No integer ratio found within the step limit. */
    return (seq_t)(-1);
}
