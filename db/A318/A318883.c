/*
 * A318883  Number of transient terms if unitary-proper-divisor-sum-function
 *          f(x) = A063919(x) is iterated and the initial value is n.
 * https://oeis.org/A318883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of transient part in iteration of the unitary
 *            proper divisor sum (A063919) starting at n
 * Range:     u8 n <= 1344, u16 n <= 9984, u32 n <= 9984, u64 n <= 9984,
 *            u128 n <= 9984, big any n
 * Helpers:   f_unitary_proper_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses Floyd’s cycle‑finding algorithm and
    a trial‑division factorisation to compute A063919(x).  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/f_unitary_proper_sum.h"

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Floyd’s cycle‑finding */
    seq_t tortoise = start;
    seq_t hare     = f_unitary_proper_sum(start);
    while (tortoise != hare) {
        tortoise = f_unitary_proper_sum(tortoise);
        hare     = f_unitary_proper_sum(f_unitary_proper_sum(hare));
    }

    /* Find cycle length λ */
    idx_t cycle_len = 1;
    seq_t meet = f_unitary_proper_sum(tortoise);
    while (meet != tortoise) {
        meet = f_unitary_proper_sum(meet);
        ++cycle_len;
    }

    /* Find transient length μ (distance from start to cycle start) */
    tortoise = start;
    hare = start;
    for (idx_t i = 0; i < cycle_len; ++i)    /* advance hare λ steps */
        hare = f_unitary_proper_sum(hare);

    idx_t transient_len = 0;
    while (tortoise != hare) {
        tortoise = f_unitary_proper_sum(tortoise);
        hare     = f_unitary_proper_sum(hare);
        ++transient_len;
    }

    return (seq_t)transient_len;
}
