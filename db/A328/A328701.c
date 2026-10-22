/*
 * A328701  Period in residues modulo n in iteration of x^2 + x + 1 starting
 *          at 0.
 * https://oeis.org/A328701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the iteration * f(k+1) = f(k)^2 + f(k) + 1 (mod
 *            n) starting with f(0)=0. * The period is the length of the cycle
 *            reached by this sequence * modulo n. The implementation uses
 *            Floyd’s cycle‑finding algorithm
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   f_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/f_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe for completeness */
    if (n == 1) return (seq_t)1;   /* f(0)=0, f(1)=0; period is 1 */

    /* Phase 1: find intersection of tortoise and hare. */
    seq_t tortoise = f_mod((seq_t)0, n);
    seq_t hare = f_mod(tortoise, n);
    while (tortoise != hare) {
        tortoise = f_mod(tortoise, n);
        hare = f_mod(f_mod(hare, n), n);
    }

    /* Phase 2: find mu (start of cycle). */
    seq_t mu = 0;
    tortoise = (seq_t)0;
    while (tortoise != hare) {
        tortoise = f_mod(tortoise, n);
        hare = f_mod(hare, n);
        ++mu;
    }

    /* Phase 3: find lambda (period). */
    seq_t lambda = 1;
    hare = f_mod(tortoise, n);
    while (hare != tortoise) {
        hare = f_mod(hare, n);
        ++lambda;
    }

    return lambda;
}
