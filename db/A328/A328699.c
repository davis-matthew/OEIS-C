/*
 * A328699  Start with 0, a(n) is the smallest number of iterations: x ->
 *          (x^2+1) mod n needed to run into a cycle.
 * https://oeis.org/A328699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the pre‑period of the iteration f(x) =
 *            (x^2 + 1) mod n starting from x = 0. The length of the
 *            pre‑period (first index i such that f(i) = f(j) for some j > i)
 *            is computed by Floyd’s cycle‑finding algorithm
 * Range:     u8 n <= 1378, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t f(seq_t x, seq_t n)
{
    /* The value x is always less than n <= 2^64-1, so it can safely
       be cast to uint64_t for the modular arithmetic.  The result is
       converted back to seq_t. */
    uint64_t ux = (uint64_t)x;
    uint64_t un = (uint64_t)n;
    __uint128_t sq = (__uint128_t)ux * ux;
    __uint128_t y  = sq + 1;
    return (seq_t)(y % un);
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* n == 1 produces immediate repetition */
        return 0;

    /* Floyd’s algorithm: find a meeting point inside the cycle */
    seq_t tortoise = f(0, n);           /* f(x0)          */
    seq_t hare     = f(f(0, n), n);     /* f(f(x0))      */
    while (tortoise != hare)
    {
        tortoise = f(tortoise, n);
        hare     = f(f(hare, n), n);
    }

    /* Find the start of the cycle (pre‑period length mu) */
    seq_t mu = 0;
    tortoise = 0;
    while (tortoise != hare)
    {
        tortoise = f(tortoise, n);
        hare     = f(hare, n);
        mu++;
    }
    return mu;
}
