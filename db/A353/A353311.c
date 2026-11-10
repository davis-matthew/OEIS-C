/*
 * A353311  Number of distinct terms encountered when A353313 is iterated, or
 *          -1 if the iteration never reaches a finite cycle.
 * https://oeis.org/A353311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = mu + lambda, where mu = tail length and lambda = cycle
 *            length of the orbit of n under A353313 (A353313(3k)=k,
 *            A353313(3k+r)=5k+3+r); found by Brent cycle detection; -1 if no
 *            cycle found within a step cap
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 95 terms through n = 9981 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A353311_step(seq_t x)
{
    seq_t k = x / 3;
    seq_t r = x % 3;
    if (r == 0)
        return k;
    return 5 * k + 3 + r;
}

seq_t a(idx_t n)
{
    const uint64_t MAX_STEPS = 5000000ULL;
    seq_t x0 = (seq_t)n;
    uint64_t power = 1, lam = 1, steps = 0;
    seq_t tortoise = x0;
    seq_t hare = A353311_step(x0);

    while (tortoise != hare) {
        if (++steps > MAX_STEPS)
            return (seq_t)-1;
        if (power == lam) {
            tortoise = hare;
            power *= 2;
            lam = 0;
        }
        hare = A353311_step(hare);
        lam++;
    }

    tortoise = x0;
    hare = x0;
    for (uint64_t i = 0; i < lam; i++)
        hare = A353311_step(hare);

    uint64_t mu = 0;
    while (tortoise != hare) {
        tortoise = A353311_step(tortoise);
        hare = A353311_step(hare);
        mu++;
    }

    return (seq_t)(mu + lam);
}
