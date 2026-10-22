/*
 * A398223  a(n) is the number of iterations for the map f(k) = k/3 if k == 0
 *          (mod 3), 4k+1 if k == 1 (mod 3), 2k-1 if k == 2 (mod 3), to first
 *          reach a previously-visited value, starting from k=n.
 * https://oeis.org/A398223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = μ(n) + λ(n) * where μ is the number of steps before the
 *            trajectory enters the cycle, * and λ is the length of the cycle.
 *            The algorithm uses Floyd’s * cycle‑finding method, which works
 *            for any finite trajectory
 * Range:     u8 n <= 15, u16 n <= 3408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   f_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/f_step.h"

seq_t a(idx_t n)
{
    /* Start value as a seq_t. */
    seq_t start = (seq_t)n;

    /* --- Floyd's cycle detection ----------------------------------- */
    /* Phase 1: find K*λ steps until tortoise == hare. */
    seq_t tortoise = f_step(start);
    seq_t hare     = f_step(f_step(start));

    while (!(tortoise == hare)) {
        tortoise = f_step(tortoise);
        hare     = f_step(f_step(hare));
    }

    /* Phase 2: find μ (first index of cycle). */
    seq_t mu_tortoise = start;
    seq_t mu_hare     = tortoise;          /* instead of hare          */
    seq_t mu = (seq_t)0;

    while (!(mu_tortoise == mu_hare)) {
        mu_tortoise = f_step(mu_tortoise);
        mu_hare     = f_step(mu_hare);
        mu++;
    }

    /* Phase 3: find λ (cycle length). */
    seq_t lambda = (seq_t)1;
    seq_t lambda_hare = f_step(mu_tortoise);   /* mu_tortoise == mu_hare */

    while (!(mu_tortoise == lambda_hare)) {
        lambda_hare = f_step(lambda_hare);
        lambda++;
    }

    /* a(n) = μ + λ. */
    return mu + lambda;
}
